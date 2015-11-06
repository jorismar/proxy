//
//  dash.cpp
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#include "dash.h"
#include "types.h"

toolsdash::Dash::Dash(std::string f){
	int i = 0;
	this->file = gf_isom_open(f.c_str(),GF_ISOM_OPEN_READ,NULL);
	this->listmp4.push_back(this->file);
	mostradados();
	fragmenta();
}

toolsdash::Dash::~Dash(){}

const char* toolsdash::Dash::gf_dasher_strip_output_dir(const char *mpd_url, const char *path){
	char c;
	const char *res;
	char *sep = strrchr(mpd_url, '/');
	if (!sep) sep = strrchr(mpd_url, '\\');
	if (!sep) return path;
	c = sep[0];
	sep[0] = 0;
	if (!strncmp(mpd_url, path, strlen(mpd_url))) {
		res = path + strlen(mpd_url) + 1;
	} else {
		res = path;
	}
	sep[0] = c;
	return res;
}
void toolsdash::Dash::gf_dash_append_segment_timeline(GF_BitStream *mpd_timeline_bs, u64 segment_start, u64 segment_end, u64 *previous_segment_duration , Bool *first_segment_in_timeline,u32 *segment_timeline_repeat_count){
	char szMPDTempLine[2048];
	u64 segment_dur = segment_end - segment_start;
	if (*previous_segment_duration == segment_dur) {
		*segment_timeline_repeat_count = *segment_timeline_repeat_count + 1;
		return;
	} 

	if (*previous_segment_duration) {
		if (*segment_timeline_repeat_count) {
			sprintf(szMPDTempLine, " r=\"%d\"/>\n", *segment_timeline_repeat_count);
		} else {
			sprintf(szMPDTempLine, "/>\n");
		}
		gf_bs_write_data(mpd_timeline_bs, szMPDTempLine, (u32) strlen(szMPDTempLine));
	}
	*previous_segment_duration = segment_dur;
	if (*first_segment_in_timeline) {
		sprintf(szMPDTempLine, "     <S t=\""LLU"\" d=\""LLU"\"", segment_start, segment_dur);
		*first_segment_in_timeline = GF_FALSE;
	} else {
		sprintf(szMPDTempLine, "     <S d=\""LLU"\"", segment_dur);
	}
	gf_bs_write_data(mpd_timeline_bs, szMPDTempLine, (u32) strlen(szMPDTempLine));
	*segment_timeline_repeat_count = 0;

}
void toolsdash::Dash::gf_dash_load_segment_timeline(toolsdash::ConfigInput * dash_cfg, GF_BitStream *mpd_timeline_bs, const char * representationID, u64 * previous_segment_duration, Bool *first_segment_in_timeline,u32 *segment_timeline_repeat_count){
	u32 i, count;
	char szRepID[100];

	*first_segment_in_timeline = GF_TRUE;
	*segment_timeline_repeat_count = 0;
	*previous_segment_duration = 0;

	count = gf_cfg_get_key_count(dash_cfg->dash_ctx, "SegmentsStartTimes");
	for (i=0; i<count; i++) {
		u64 start, dur;
		const char *fileName = gf_cfg_get_key_name(dash_cfg->dash_ctx, "SegmentsStartTimes", i);
		const char *MPDTime = gf_cfg_get_key(dash_cfg->dash_ctx, "SegmentsStartTimes", fileName);
		if (!fileName)
			break;

		sscanf(MPDTime, ""LLU"-"LLU"@%s", &start, &dur, szRepID);
		if (strcmp(representationID, szRepID)) continue;

		gf_dash_append_segment_timeline(mpd_timeline_bs, start, start+dur, previous_segment_duration, first_segment_in_timeline, segment_timeline_repeat_count);
	}

}
	
void toolsdash::Dash::seta_entrada(ConfigFrag *dash_inputs, char *name, u32 *nb_dash_inputs){
	GF_DashSegmenterInput *di;
	ConfigFrag di;
	dash_inputs = gf_realloc(dash_inputs, sizeof(GF_DashSegmenterInput) * (*nb_dash_inputs + 1) );
	memset(&dash_inputs[*nb_dash_inputs], 0, sizeof(GF_DashSegmenterInput) );
	di = &dash_inputs[*nb_dash_inputs];
	(*nb_dash_inputs)++;

	di->file_name = name;
	if (!di->representationID) {
		char szRep[100];
		sprintf(szRep, "%d", *nb_dash_inputs);
		di->representationID = gf_strdup(szRep);
	}
	return dash_inputs;
}
GF_Err toolsdash::Dash::dasher_isom_segment_file_lavid(GF_ISOFile *input, char * output_file, toolsdash::ConfigInput *dash_cfg, toolsdash::ConfigFrag *dash_input, Bool first_in_set){
	GF_Err e = GF_OK;
	u8 NbBits;
	u32 i, TrackNum, descIndex, j, count, nb_sync, ref_track_id, nb_tracks_done;
	u32 defaultDuration, defaultSize, defaultDescriptionIndex, defaultRandomAccess, nb_samp, nb_done;
	u32 nb_video, nb_audio, nb_text, nb_scene, mpd_timescale;
	u8 defaultPadding;
	u16 defaultDegradationPriority;
	char sOpt[100], sKey[100];
	char szCodecs[200], szCodec[100];
	u32 first_seg, cur_seg, fragment_index, max_sap_type;
	GF_ISOFile *output, *bs_switch_segment;
	GF_ISOSample *sample, *next;
	GF_List *fragmenters;
	u64 MaxFragmentDuration, MaxSegmentDuration, period_duration;
	Double segment_start_time, SegmentDuration, maxFragDurationOverSegment;
	u64 presentationTimeOffset = 0;
	Double file_duration, max_segment_duration;
	u32 nb_segments, width, height, sample_rate, nb_channels, sar_w, sar_h, fps_num, fps_denum, startNumber;
	u32 nbFragmentInSegment = 0;
	char *langCode = NULL;
	u32 index_start_range, index_end_range;
	Bool force_switch_segment = GF_FALSE;
	Bool switch_segment = GF_FALSE;
	Bool split_seg_at_rap = dash_cfg->segments_start_with_rap;
	Bool split_at_rap = GF_FALSE;
	Bool has_rap = GF_FALSE;
	Bool next_sample_rap = GF_FALSE;
	Bool flush_all_samples = GF_FALSE;
	Bool simulation_pass = GF_FALSE;
	Bool init_segment_deleted = GF_FALSE;
	Bool first_segment_in_timeline = GF_TRUE;
	Bool store_utc = GF_FALSE;
	u64 previous_segment_duration = 0;
	u32 segment_timeline_repeat_count = 0;
	u64 last_ref_cts = 0;
	u64 start_range, end_range, file_size, init_seg_size, ref_track_first_dts, ref_track_next_cts;
	u32 tfref_timescale = 0;
	u32 bandwidth = 0;
	toolsdash::GF_ISOMTrackFragmenter *tf, *tfref;
	GF_BitStream *mpd_bs = NULL;
	GF_BitStream *mpd_timeline_bs = NULL;
	char szMPDTempLine[2048];
	char SegmentName[GF_MAX_PATH];
	char RepSecName[200];
	char RepURLsSecName[200];
	const char *opt;
	Double max_track_duration = 0;
	Bool bs_switching_is_output = GF_FALSE;
	Bool store_dash_params = GF_FALSE;
	Bool dash_moov_setup = GF_FALSE;
	Bool segments_start_with_sap = GF_TRUE;
	Bool first_sample_in_segment = GF_FALSE;
	u32 *segments_info = NULL;
	u32 nb_segments_info = 0;
	u32 protected_track = 0;
	Double min_seg_dur, max_seg_dur, total_seg_dur, last_seg_dur;
	Bool is_bs_switching = GF_FALSE;
	Bool use_url_template = (Bool)dash_cfg->use_url_template;
	const char *seg_rad_name = dash_cfg->seg_rad_name;
	const char *seg_ext = dash_cfg->seg_ext;
	const char *bs_switching_segment_name = NULL;
	u64 generation_start_ntp = 0;
	u64 generation_start_utc = 0;
	u64 ntpts = 0;
	SegmentName[0] = 0;
	SegmentDuration = 0;
	nb_samp = 0;
	fragmenters = NULL;
	if (!seg_ext) seg_ext = "m4s";

	if (dash_cfg->real_time && dash_cfg->dash_ctx) {
		u32 sec, frac;
		opt = gf_cfg_get_key(dash_cfg->dash_ctx, "DASH", "GenerationNTP");
		sscanf(opt, "%u:%u", &sec, &frac);
		generation_start_ntp = sec;
		generation_start_ntp <<= 32;
		generation_start_ntp |= frac;

		generation_start_utc = sec - GF_NTP_SEC_1900_TO_1970;
		generation_start_utc *= 1000;
		generation_start_utc += ((u64) frac) * 1000 / 0xFFFFFFFFUL;
	}
	
	if (dash_cfg->insert_utc) {
		ntpts = gf_net_get_ntp_ts();
		GF_LOG(GF_LOG_DEBUG, GF_LOG_DASH, ("[DASH] Sampling NTP TS "LLU" at "LLU" us, at UTC "LLU"\n", ntpts, gf_sys_clock_high_res(), gf_net_get_utc()));
	}

	bs_switch_segment = NULL;
	if (dash_cfg->bs_switch_segment_file) {
		bs_switch_segment = gf_isom_open(dash_cfg->bs_switch_segment_file, GF_ISOM_OPEN_READ, NULL);
		if (bs_switch_segment) {
			bs_switching_segment_name = gf_dasher_strip_output_dir(dash_cfg->mpd_name, dash_cfg->bs_switch_segment_file);
			is_bs_switching = GF_TRUE;
		}
	}
	return e;

	sprintf(RepSecName, "Representation_%s", dash_input ? dash_input->representationID : "");
	sprintf(RepURLsSecName, "URLs_%s", dash_input ? dash_input->representationID : "");

	bandwidth = dash_input ? dash_input->bandwidth : 0;
	file_duration = 0;

	startNumber = 1;

	//create output file
	/*need to precompute bandwidth*/
	if (!bandwidth && seg_rad_name && strstr(seg_rad_name, "$Bandwidth$")) {
		for (i=0; i<gf_isom_get_track_count(input); i++) {
			Double tk_dur = (Double) gf_isom_get_media_duration(input, i+1);
			tk_dur /= gf_isom_get_media_timescale(input, i+1);
			if (file_duration < tk_dur ) {
				file_duration = tk_dur;
			}
		}
		bandwidth = (u32) (gf_isom_get_file_size(input) / file_duration * 8);
	}

	if (dash_cfg->dash_ctx) {
		opt = gf_cfg_get_key(dash_cfg->dash_ctx, RepSecName, "Setup");
		if (!opt || stricmp(opt, "yes") ) {
			store_dash_params=GF_TRUE;
			gf_cfg_set_key(dash_cfg->dash_ctx, RepSecName, "ID", dash_input->representationID);
		}
	}

	opt = dash_cfg->dash_ctx ? gf_cfg_get_key(dash_cfg->dash_ctx, RepSecName, "InitializationSegment") : NULL;
	if (opt) {
		output = gf_isom_open(opt, GF_ISOM_OPEN_CAT_FRAGMENTS, dash_cfg->tmpdir);
		dash_moov_setup = GF_TRUE;
	} else {
		gf_media_mpd_format_segment_name(GF_DASH_TEMPLATE_INITIALIZATION, is_bs_switching, SegmentName, output_file, dash_input->representationID, dash_input->nb_baseURL ? dash_input->baseURL[0] : NULL, seg_rad_name, !stricmp(seg_ext, "null") ? NULL : "mp4", 0, bandwidth, 0, dash_cfg->use_segment_timeline);
		output = gf_isom_open(SegmentName, GF_ISOM_OPEN_WRITE, dash_cfg->tmpdir);
	}
	if (!output) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_AUTHOR, ("[ISOBMF DASH] Cannot open %s for writing\n", opt ? opt : SegmentName));
		e = gf_isom_last_error(NULL);
		exit(1);
	}

	if (store_dash_params) {
		const char *name;

		if (!gf_cfg_get_key(dash_cfg->dash_ctx, "DASH", "SegmentTemplate"))
			gf_cfg_set_key(dash_cfg->dash_ctx, "DASH", "SegmentTemplate", seg_rad_name);
		gf_cfg_set_key(dash_cfg->dash_ctx, RepSecName, "Source", gf_isom_get_filename(input) );

		gf_cfg_set_key(dash_cfg->dash_ctx, RepSecName, "Setup", "yes");
		name = SegmentName;
		if (bs_switch_segment) name = gf_isom_get_filename(bs_switch_segment);
		gf_cfg_set_key(dash_cfg->dash_ctx, RepSecName, "InitializationSegment", name);

		/*store BS flag per rep - it should be stored per adaptation set but we dson't have a key for adaptation sets right now*/
		if (/*first_in_set && */ is_bs_switching)
			gf_cfg_set_key(dash_cfg->dash_ctx, RepSecName, "BitstreamSwitching", "yes");
	} else if (dash_cfg->dash_ctx) {
		opt = gf_cfg_get_key(dash_cfg->dash_ctx, RepSecName, "BitstreamSwitching");
		if (opt && !strcmp(opt, "yes")) {
			is_bs_switching = GF_TRUE;
			if (bs_switch_segment)
				gf_isom_delete(bs_switch_segment);
			bs_switch_segment = output;
			bs_switching_is_output = GF_TRUE;
			bs_switching_segment_name = gf_dasher_strip_output_dir(dash_cfg->mpd_name, gf_isom_get_filename(bs_switch_segment));
		}

		opt = gf_cfg_get_key(dash_cfg->dash_ctx, RepSecName, "Bandwidth");
		if (opt) sscanf(opt, "%u", &bandwidth);

		if (dash_cfg->use_segment_timeline) {
			opt = gf_cfg_get_key(dash_cfg->dash_ctx, RepSecName, "NbSegmentsRemoved");
			if (opt) {
				u32 nb_removed = atoi(opt);
				startNumber = 1 + nb_removed;
			}
		}
	}
	mpd_bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
	//if segment alignment not set or if first in AS, create SegmentTimeline
	if (dash_cfg->use_segment_timeline && (first_in_set || dash_cfg->segment_alignment_disabled) ) {
		mpd_timeline_bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
		sprintf(szMPDTempLine, "    <SegmentTimeline>\n");
		gf_bs_write_data(mpd_timeline_bs, szMPDTempLine, (u32) strlen(szMPDTempLine));

		if (dash_cfg->dash_ctx) {
			gf_dash_load_segment_timeline(dash_cfg, mpd_timeline_bs, dash_input->representationID, &previous_segment_duration, &first_segment_in_timeline, &segment_timeline_repeat_count);
		}
	}

	nb_sync = 0;
	nb_samp = 0;
	fragmenters = gf_list_new();

	if (! dash_moov_setup) {							//!dash_cfg->pssh_moof
		e = gf_isom_clone_movie(input, output, GF_FALSE, GF_FALSE, dash_cfg->pssh_moof);
		if (e) exit(1);

		/*because of movie fragments MOOF based offset, ISOM <4 is forbidden*/
		gf_isom_set_brand_info(output, GF_4CC('i','s','o','5'), 1);
		gf_isom_modify_alternate_brand(output, GF_4CC('i','s','o','m'), 0);
		gf_isom_modify_alternate_brand(output, GF_4CC('i','s','o','1'), 0);
		gf_isom_modify_alternate_brand(output, GF_4CC('i','s','o','2'), 0);
		gf_isom_modify_alternate_brand(output, GF_4CC('i','s','o','3'), 0);
		gf_isom_modify_alternate_brand(output, GF_ISOM_BRAND_MP41, 0);
		gf_isom_modify_alternate_brand(output, GF_ISOM_BRAND_MP42, 0);

	}

	MaxFragmentDuration = (u32) (dash_cfg->dash_scale * dash_cfg->fragment_duration);
	MaxSegmentDuration = (u32) (dash_cfg->dash_scale * dash_cfg->segment_duration);

	/*in single segment mode, only one big SIDX is written between the end of the moov and the first fragment.
	To speed-up writing, we do a first fragmentation pass without writing any sample to compute the number of segments and fragments per segment
	in order to allocate / write to file the sidx before the fragmentation. The sidx will then be rewritten when closing the last segment*/
	if (dash_cfg->single_file_mode==1) {
		simulation_pass = GF_TRUE;
		seg_rad_name = NULL;
	}
	/*if single file is requested, store all segments in the same file*/
	else if (dash_cfg->single_file_mode==2) {
		seg_rad_name = NULL;
	}

	index_start_range = index_end_range = 0;

	tf = tfref = NULL;
	file_duration = 0;
	width = height = sample_rate = nb_channels = sar_w = sar_h = fps_num = fps_denum = 0;
	langCode = NULL;
	szCodecs[0] = 0;
}

void toolsdash::Dash::fragmenta(){
	int i = 0;
	double dash_duration = 4;
	double interleaving_time = 4;
	u32 subsegs_per_sidx;
	Bool single_segment = GF_FALSE;
	Bool memory_frags = GF_TRUE;
	GF_DashSwitchingMode bitstream_switching_mode = GF_DASH_BSMODE_DEFAULT;
	u32 initial_moof_sn = 0;
	GF_DashProfile dash_profile = GF_DASH_PROFILE_LIVE;
	u32 dash_scale = 1000;
	GF_Config *dash_ctx = NULL;
	GF_DASHSegmenter *dasher;
	//GF_DashSegInput *inputdash;
	GF_DashSegmenterInput *dash_inputs = NULL;
	Bool seg_at_rap = GF_FALSE;
	Bool frag_at_rap = GF_FALSE;
	Bool daisy_chain_sidx = GF_FALSE;
	u32 segment_marker = GF_FALSE;
	char cprt[] = "Lavid";
	char **mpd_base_urls = NULL;
	const char *dash_more_info = NULL;
	const char *dash_source = NULL;
	char seg_name[]= "lavid";
	char *seg_ext = NULL;
	char szMPD[GF_MAX_PATH], *sep;
	char *dash_ctx_file = NULL;
	const char *dash_profile_extension = NULL;
	Bool use_url_template = GF_FALSE;
	Bool segment_timeline = GF_FALSE;
	Bool single_file = GF_FALSE;
	Bool dash_duration_strict = GF_FALSE;
	Bool force_new = GF_FALSE;
	Double min_buffer = 1.5;
	Double mpd_update_time = GF_FALSE;
	Double mpd_live_duration = 0;
	GF_Err e;
	u32 nb_mpd_base_urls = 0;
	u32	time_shift_depth = 0;
	u32 nb_dash_inputs = 1;
	s32 ast_offset_ms = 0;
	u64 initial_tfdt = 0;
	Bool no_fragments_defaults = GF_FALSE;
	Bool pssh_in_moof = GF_FALSE;
	Bool samplegroups_in_traf = GF_FALSE;
	Bool single_traf_per_moof = GF_FALSE;
	Bool insert_utc = GF_FALSE;
	Bool frag_real_time = GF_FALSE;
	GF_DashDynamicMode dash_mode=GF_DASH_STATIC;

		strcat(szMPD,"teste");
		strcat(szMPD, ".mpd");
		dash_ctx = gf_cfg_new(NULL, NULL);
		if (dash_ctx_file) {
			if (force_new)
				gf_delete_file(dash_ctx_file);
			dash_ctx = gf_cfg_force_new(NULL, dash_ctx_file);
		}
		dasher = gf_dasher_new(szMPD,dash_profile,NULL,dash_scale,dash_ctx);
		e = gf_dasher_set_info(dasher,"title",cprt,"s","c:/");
		if(e){
			PRINT("Erro!");
		}	

		for (i=0; i < nb_mpd_base_urls; i++) {
			e = gf_dasher_add_base_url(dasher, mpd_base_urls[i]);
			if (e){
				PRINT("Erro np_mpd_base_urls");
			} 
		}


		e = gf_dasher_enable_url_template(dasher, (Bool) use_url_template, seg_name, seg_ext);
		if(!e) e = gf_dasher_enable_segment_timeline(dasher,segment_timeline);
		if (!e) e = gf_dasher_enable_single_segment(dasher, single_segment);
		if (!e) e = gf_dasher_enable_single_file(dasher, single_file);
		if (!e) e = gf_dasher_set_switch_mode(dasher, bitstream_switching_mode);
		if (!e) e = gf_dasher_set_durations(dasher, dash_duration, dash_duration_strict, interleaving_time);
		if (!e) e = gf_dasher_enable_rap_splitting(dasher, seg_at_rap, frag_at_rap);
		if (!e) e = gf_dasher_set_segment_marker(dasher, segment_marker);
		//if (!e) e = gf_dasher_enable_sidx(dasher, (subsegs_per_sidx >=0 ) ? 1 : 0, subsegs_per_sidx, daisy_chain_sidx);
		if (!e) e = gf_dasher_set_dynamic_mode(dasher, dash_mode, mpd_update_time, time_shift_depth, mpd_live_duration);
		if (!e) e = gf_dasher_set_min_buffer(dasher, min_buffer);
		if (!e) e = gf_dasher_set_ast_offset(dasher, ast_offset_ms);
		if (!e) e = gf_dasher_enable_memory_fragmenting(dasher, memory_frags);
		if (!e) e = gf_dasher_set_initial_isobmf(dasher, initial_moof_sn, initial_tfdt);
		if (!e) e = gf_dasher_configure_isobmf_default(dasher, no_fragments_defaults, pssh_in_moof, samplegroups_in_traf, single_traf_per_moof);
		if (!e) e = gf_dasher_enable_utc_ref(dasher, insert_utc);
		if (!e) e = gf_dasher_enable_real_time(dasher, frag_real_time);
		if (!e) e = gf_dasher_set_profile_extension(dasher, dash_profile_extension);
		//for(int i = 0; i<nb_dash_inputs;i++){
		//	gf_dasher_add_input(dasher,&dash_inputs[i]);
		//}
		e = gf_dasher_process(dasher,dash_duration);	
		//PRINT("Time scale: "<<isom_get_track_duration_from_samples_in_timescalee(this->file,2));
		//pegando track
		GF_ISOFile * aux;
		
		//aux = gf_isom_open("segmento.mp4",GF_ISOM_OPEN_WRITE,"/Users/Josue/Documents");
		e = gf_media_fragment_file(this->file,"segmento.mp4",4.0);	
}
void toolsdash::Dash::recebeudp(){

	const char *udpts = NULL;
	const char *grab_ifce = NULL;
	GF_Err e;
	u32 i;
	char *dst = NULL;
	char *ip = NULL;
	u16 port = 7000;
	u32 load_type=0;
	u32 check;
	u32 ttl = 1;
	u32 path_mtu = 1450;
	s32 next_time;
	u64 last_src_modif, mod_time;
	char *src_name = NULL;
	Bool run, has_carousel, no_rap;
	u16 socket_port=1234;
	GF_Socket *socket = NULL;
	char *update_buffer = NULL;
	u32 update_buffer_size = 0;
	u16 aggregate_on_stream;
	Bool adjust_carousel_time, force_rap, aggregate_au, discard_pending, signal_rap, version_inc;
	Bool update_context;
	u32 period, ts_delta, signal_critical;
	u16 es_id;
	e = GF_OK;
	es_id = 0;

	int controle = 1;
	//levanto um socket aqui para receber os pacotes udp
	socket = gf_sk_new(GF_SOCK_TYPE_UDP);
	e = gf_sk_bind(socket,NULL,socket_port,NULL,0,0);
		if(e!=GF_OK){
			controle = 0;
			PRINT("Erro na criacao do socket");
			if(socket)gf_sk_del(socket);
			socket = NULL;
		}
	while(controle){
		char buffer[2049];
		u32 bytes_read;
		u32 update_length;
		u32 bytes_received;
		e = gf_sk_receive(socket,buffer,2048,0,&bytes_read);
		PRINT("Bytes lidos "<<bytes_read);
		if(e == GF_OK){
			PRINT("GF_OK");
			u32 hdr_length = 0;
			u8 cmd_type = buffer[0];
			bytes_received = 0;
		}

	}

	//gra_live_m2ts();
	//dash_inputs = set_dash_input(dash_inputs,"ex.mp4",&n_entrada);
	//gf_dasher_new();
	//fragmento = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/segmento.mp4",GF_ISOM_OPEN_EDIT,NULL);

	//PRINT("Teste"<<fragmento->fileName);
	//if (file_duration < ((Double) gf_isom_get_media_duration(input, i+1)) / tf->TimeScale ) {
	//file_duration = ((Double) gf_isom_get_media_duration(input, i+1)) / tf->TimeScale;
	//gf_isom_start_fragment();
	//aux = gf_isom_new()
	//gf_isom_set_movie_duration(aux,3);
}

void toolsdash::Dash::mostradados(){
	GF_ISOFile *aux;
	aux = this->file;
	//aux = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/ex.mp4",GF_ISOM_OPEN_READ,NULL);
	PRINT("Dados do arquivo mp4");
	PRINT("Nome do arquivo "<<aux->fileName);
	PRINT(""<<aux->brand->size);
	PRINT("Dados do moov");
	PRINT("Tamanho: "<<aux->moov->size<<"\n");
	PRINT("\nDados do mvhd");
	PRINT("\nTamanho: "<<aux->moov->mvhd->size);
	PRINT("CreationTime: "<<aux->moov->mvhd->creationTime);
	PRINT("ModificationTime: "<<aux->moov->mvhd->modificationTime);
	PRINT("TimeScale: "<<aux->moov->mvhd->timeScale);
	PRINT("Duration: "<<aux->moov->mvhd->duration);
	PRINT("NextTrackID: "<<aux->moov->mvhd->nextTrackID);
	PRINT("PreferredRate: "<<aux->moov->mvhd->preferredRate);
    PRINT("PreferredVolume: "<<aux->moov->mvhd->preferredVolume);
    PRINT("Preview Time: "<<aux->moov->mvhd->previewTime);
    PRINT("Preview Duration: "<<aux->moov->mvhd->previewDuration);
    PRINT("Current Time: "<<aux->moov->mvhd->currentTime<<"\n");
    PRINT("Dados do mdat");
    PRINT("Tamanho: "<<aux->mdat->size<<"\n");
    PRINT("Duracao do track "<<gf_isom_get_media_duration(aux,3));
    PRINT("Numero de tracks "<<gf_isom_get_track_count(aux));
    PRINT("TimeScale da trak 1 "<<gf_isom_get_media_timescale(aux,1));
}