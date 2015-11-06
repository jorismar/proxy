//
//  dash.h
//  MPEG-TS-to-MPEG-DASH
//
//  Created by Josue on 16/10/15.
#ifndef __MPEGTStoMPEGDASH__dash__
#define __MPEGTStoMPEGDASH__dash__
#include <vector>
#include <string>
extern "C"{
	#include <gpac/isomedia.h>
	#include <gpac/media_tools.h>
	#include <gpac/internal/isomedia_dev.h>
	#include <gpac/internal/media_dev.h>
	#include <gpac/constants.h>
}

#include <gpac/scene_engine.h>
#include <gpac/filestreamer.h>
#include <gpac/mpegts.h>
#include <gpac/setup.h>
#include "types.h"
#include "buffer.h"
#include <gpac/config_file.h>
#include <gpac/network.h>
static GF_Err MP4_API_IO_Err;

namespace toolsdash{

enum{
	STREAM_AUDIO = 1,
	STREAM_VIDEO = 2,
	STREAM_DESCONHECIDO = 3
};

class Component{
public:
	u32 ID;/*audio/video/text/ ...*/
    u32 media_type;/*audio/video/text/ ...*/
    char szCodec[50];
    /*for video */
    u32 width, height, fps_num, fps_denum, sar_num, sar_denum, max_sap;
        
    /*for audio*/
    u32 sample_rate, channels;
	/*apply to any media. We use 5 bytes because we may use copy data converted from gf_4cc_to_str which is 5 bytes*/
	char *lang;
	Component();
	~ Component();
};

class GF_ISOMTrackFragmenter{
public:
	Bool is_ref_track;
	Bool done;
	u32 TrackID;
	u32 SampleNum, SampleCount;
	u64 FragmentLength;
	u32 OriginalTrack;
	u32 finalSampleDescriptionIndex;
	u32 TimeScale, MediaType, DefaultDuration;
	u64 last_sample_cts, next_sample_dts, InitialTSOffset;
	Bool all_sample_raps, splitable;
	u32 split_sample_dts_shift;
	s32 media_time_to_pres_time_shift;
	u64 min_cts_in_segment;
	 GF_ISOMTrackFragmenter();
	~ GF_ISOMTrackFragmenter();
};

class ConfigFrag{
public:
	char *file_name;
    char representationID[100];
    char *periodID;
    Double media_duration; /*forced media duration*/
    u32 nb_baseURL;
    char **baseURL;
    char *xlink;
    char *role;
    u32 nb_rep_descs;
    char **rep_descs;
    u32 nb_as_descs;
    char **as_descs;
    u32 nb_as_c_descs;
    char **as_c_descs;
    u32 nb_p_descs;
    char **p_descs;
    u32 bandwidth;
    u32 dependency_bandwidth;
    char *dependencyID;
        
    /*if 0, input is disabled*/
    u32 adaptation_set;
    /*if 0, input is disabled*/
    u32 period;
    u32 group_id;
    /*only set for the first representation in the AdaptationSet*/
    u32 nb_rep_in_adaptation_set;
    Double period_duration;
    
    /*media-format specific functions*/
    /*assigns the different media to the same adaptation set or group than the input_idx one*/
    //GF_Err (* dasher_input_classify) (GF_DashSegInput *dash_inputs, u32 nb_dash_inputs, u32 input_idx, u32 *current_group_id, u32 *max_sap_type);
    //GF_Err ( *dasher_get_components_info) (GF_DashSegInput *dash_input, GF_DASHSegmenter *opts);
    //GF_Err ( *dasher_create_init_segment) (GF_DashSegInput *dash_inputs, u32 nb_dash_inputs, u32 adaptation_set, char *szInitName, const char *tmpdir, GF_DASHSegmenter *dash_opts, GF_DashSwitchingMode bs_switch_mode, Bool *disable_bs_switching);
    //GF_Err ( *dasher_segment_file) (GF_DashSegInput *dash_input, const char *szOutName, GF_DASHSegmenter *opts, Bool first_in_set);
        
    /*shall be set after call to dasher_input_classify*/
    char szMime[50];
        
    u32 single_track_num;

    //information for scalability
    u32 trackNum, lower_layer_track, nb_representations, idx_representations;
    //increase sequence number between consecutive segments by this amount (for scalable reps)
    u32 moof_seqnum_increase;
        
    u32 protection_scheme_type;
        
    Double segment_duration;
    /*all these shall be set after call to dasher_get_components_info*/
    Double duration;
    Component components[20];
    u32 nb_components;
        
    //spatial info for tiling
    u32 x, y, w, h;
        
    Bool disable_inband_param_set;
        
    Bool period_id_not_specified;
        
    Bool init_segment_generated;
    char *init_seg_url;
    Bool use_bs_switching;
        
        
    Bool get_component_info_done;
    //cached isobmf input
    GF_ISOFile *isobmf_input;
	ConfigFrag();
	~ConfigFrag();
	
};

class ConfigInput{
public:
	char *mpd_name;
	//GF_DashSegInput *inputs;
	u32 nb_inputs;
	GF_DashProfile profile;
	char *title;
	char *location;
	char *source;
	char *copyright;
	char *moreInfoURL;
	//has to be freed
	char **base_urls;
	u32 nb_base_urls;

	u32 use_url_template;
	Bool use_segment_timeline;
	Bool single_segment;
	Bool single_file;
	GF_DashSwitchingMode bitstream_switching_mode;
	Bool segments_start_with_rap;

	Double segment_duration;
	Bool segment_duration_strict;
	Double fragment_duration;
	//has to be freed
	char *seg_rad_name;
	const char *seg_ext;
	u32 segment_marker_4cc;
	Bool enable_sidx;
	u32 subsegs_per_sidx;
	Bool daisy_chain_sidx;

	Bool fragments_start_with_rap;
	char *tmpdir;
	GF_DashDynamicMode dash_mode;
	Double mpd_update_time;
	s32 time_shift_depth;
	Double min_buffer;
	s32 ast_offset_ms;
	u32 dash_scale;
	Bool fragments_in_memory;
	u32 initial_moof_sn;
	u64 initial_tfdt;
	Bool no_fragments_defaults;
	Bool pssh_moof;
	Bool samplegroups_in_traf;
	Bool single_traf_per_moof;
	Double mpd_live_duration;
	Bool insert_utc;
	Bool real_time;
	const char *dash_profile_extension;
	
	GF_Config *dash_ctx;

	Double subduration;

	FILE *mpd;
	Bool segment_alignment_disabled;
	u32 single_file_mode;
	const char *bs_switch_segment_file;
	Bool inband_param_set;
	Bool force_period_end;

	/*set if seg_rad_name depends on input file name (had %s in it). In this case, SegmentTemplate cannot be used at adaptation set level*/
	Bool variable_seg_rad_name;

	/*duplicate ContentProtection elemnt in adaptation set for DRM and profiles compatibility*/
	Bool content_protection_in_adaptation_set;

	Double max_segment_duration;
	ConfigInput();
	~ConfigInput();
	
};

class Dash{
public:
	GF_ISOFile *file;
	GF_BitStream *stream;
	std::vector <GF_ISOFile*> listmp4;
	Component *comp;
	GF_ISOMTrackFragmenter *gf_track_frag;
	ConfigFrag *conf_frag;
	ConfigInput *conf_input;
	Buffer *buffervideo;
	Buffer *bufferaudio;
	Dash(std::string);
	~Dash();
	void mostradados();
	void fragmenta();
	void recebeudp();
	void seta_entrada(ConfigFrag *, char *, u32);
	GF_ISOFile * captura_stream(char *, u32);
	GF_Err dasher_isom_segment_file_lavid(GF_ISOFile *, char *, ConfigInput *, ConfigFrag *, Bool);
	const char *gf_dasher_strip_output_dir(const char *, const char *);
	void gf_dash_load_segment_timeline(toolsdash::ConfigInput *, GF_BitStream *, const char *, u64 *, Bool *,u32 *);
	void gf_dash_append_segment_timeline(GF_BitStream *, u64, u64, u64 *, Bool *,u32 *);
};

}// fim do namespace
#endif /* defined(MPEGTS-to-MPEG-DASH__dash) */
