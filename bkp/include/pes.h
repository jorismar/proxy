#ifndef PES_H
#define PES_H
/*
#include "mpegts.h"

typedef struct {
    unsigned int start_code;
    t_byte stream_id;
    unsigned short packet_length;
    tOptionalHeader optional;
} tPES_HeaderField;

typedef struct {
    // 2b Marker = '10'
    t_byte scramb_control;
    bool priority;
    bool data_alignment;
    bool copyright;
    bool original;              // 0 = Copy, 1 = Original

    t_byte pts_dts_flag;        // 00 = no PTS or DTS, 01 is forbidden, 10 = only PTS, 11 = both present
    bool escr_flag;
    bool es_rate_flag;
    bool dsm_trick_mode_flag;
    bool add_copy_info_flag;
    bool crc_flag;
    bool extension_flag;

    unsigned char header_length;
    tExtraOptionalHeader extra;
    // Stuffing Bytes
} tOptionalHeader;

typedef struct {
    t_byte pts[5];
    t_byte dts[5];
    t_byte escr[6];
    unsigned int es_rate;
    t_byte dsm_trick_mode;
    t_byte add_copy_info;
    short prev_pes_crc;
    
    bool priv_data_flag;
    bool header_field_flag;
    bool program_pkt_seq_count_flag;
    bool pstd_buff_flag;
    // 3b reserved
    bool extension_flag;
    
    tPES_ExtensionField extension;
} tExtraOptionalHeader;

typedef struct {
    t_byte priv_data[16];
    t_byte pkt_header_field;
    t_byte progam_pkt_seq_count;
    t_byte pstd_buff[2];
    t_byte exten_field_length;
    t_byte * exten_field_data;
} tPES_ExtensionField;

class PES {
    private:
        tPES_HeaderField header;
        DataPacket * data;
        
    public:
        PES();
        virtual ~PES();
};*/

#endif