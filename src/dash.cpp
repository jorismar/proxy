//
//  dash.cpp
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#include "dash.h"

extern "C"{
	#include "dash_segmenter.h"	
}
#include "types.h"

Dash::Dash(GF_ISOFile *f){
	this->file = f;
	int i = 0;
	this->file = gf_isom_open("/Users/Josue/Documents/MPEG-TS-to-MPEG-DASH/midia/ex.mp4",GF_ISOM_OPEN_READ,NULL);
	mostradados();
	fragmenta();
}

Dash::~Dash(){}

void Dash::fragmenta(){
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
	GF_DashSegmenterInput *dash_inputs = NULL;
	char cprt[] = "Lavid";
	char **mpd_base_urls = NULL;
	const char *dash_more_info = NULL;
	const char *dash_source = NULL;
	char seg_name[]= "lavid";
	char *seg_ext = NULL;
	char szMPD[GF_MAX_PATH], *sep;
	char *dash_ctx_file = NULL;
	Bool use_url_template = GF_FALSE;
	Bool segment_timeline = GF_FALSE;
	Bool single_file = GF_FALSE;
	Bool dash_duration_strict = GF_FALSE;
	Bool force_new = GF_FALSE;
	GF_Err e;
	u32 nb_mpd_base_urls = 0;

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
		/*if (!e) e = gf_dasher_set_durations(dasher, dash_duration, dash_duration_strict, interleaving_time);
		if (!e) e = gf_dasher_enable_rap_splitting(dasher, seg_at_rap, frag_at_rap);
		if (!e) e = gf_dasher_set_segment_marker(dasher, segment_marker);
		if (!e) e = gf_dasher_enable_sidx(dasher, (subsegs_per_sidx>=0) ? 1 : 0, (u32) subsegs_per_sidx, daisy_chain_sidx);
		if (!e) e = gf_dasher_set_dynamic_mode(dasher, dash_mode, mpd_update_time, time_shift_depth, mpd_live_duration);
		if (!e) e = gf_dasher_set_min_buffer(dasher, min_buffer);
		if (!e) e = gf_dasher_set_ast_offset(dasher, ast_offset_ms);
		if (!e) e = gf_dasher_enable_memory_fragmenting(dasher, memory_frags);
		if (!e) e = gf_dasher_set_initial_isobmf(dasher, initial_moof_sn, initial_tfdt);
		if (!e) e = gf_dasher_configure_isobmf_default(dasher, no_fragments_defaults, pssh_in_moof, samplegroups_in_traf, single_traf_per_moof);
		if (!e) e = gf_dasher_enable_utc_ref(dasher, insert_utc);
		if (!e) e = gf_dasher_enable_real_time(dasher, frag_real_time);
		if (!e) e = gf_dasher_set_profile_extension(dasher, dash_profile_extension);
		//dash_inputs = */
		//e = gf_dasher_process(dasher,dash_duration);

		//dasher_isom_segment_file(this->file,"fragmento",dasher,NULL,GF_OK);
}

void Dash::recebeudp(){

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

void Dash::mostradados(){
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