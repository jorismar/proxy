#ifndef MPEGTS_H
#define MPEGTS_H

#include "types.h"
#include "datapacket.h"

#define TS_PACKET_SIZE 188

typedef struct {
    // 1º Byte
    t_byte sync_byte;

    // 2º e 3º Byte
    bool   trans_error;
    bool   payload_start;
    bool   trans_prior;
    short  pid;

    // 4º Byte
    t_byte scramb_control;
    bool   adapt_control;
    bool   pload_control;
    t_byte contin_counter;
} tHeaderField;

typedef struct {
    unsigned long base;
    // 6b Reserved
    unsigned short extension;
} tPCRField;

typedef struct {
    unsigned long base;
    // 6b Reserved
    unsigned short extension;
} tOPCRField;

typedef struct {
    t_byte countdown;
} tSpliceCountdownField;

typedef struct {
    unsigned char length;
    t_byte * data;
} tPrivDataField;

typedef struct {
    bool valid_flag;
    short offset;
} tLTWField;

typedef struct {
    // 2b reserved
    unsigned int rate;     // 22b
} tPlecewiseField;

typedef struct {
    t_byte type;
    t_byte dts_next_au_1;   // 3b
    bool marker1;           // 1b
    short dts_next_au_2;   // 15b
    bool marker2;           // 1b
    short dts_next_au_3;   // 15b
} tSpliceField;

typedef struct {
    unsigned char length;
    bool ltw_flag;
    bool plecew_flag;
    bool splice_flag;
    // 5 bits reserved
    tLTWField ltw;
    tPlecewiseField plecewise;
    tSpliceField splice;
} tExtensionField;

typedef struct {
    unsigned char length;
    bool discontinuity;
    bool random_access;
    bool es_priority;
    
    bool pcr_flag;
    bool opcr_flag;
    bool splic_flag;
    bool priv_data_flag;
    bool extension_flag;
    
    tPCRField pcr;
    tOPCRField opcr;
    tSpliceCountdownField splice;
    tPrivDataField transp_private;
    tExtensionField extension;
    
    // Stuffing Bytes
} tAdaptationField;

typedef struct {
    unsigned int length;
    DataPacket * data;
} tPayloadField;

class MPEGTS {
    private:
        tHeaderField header;
        tAdaptationField adaptation;
        tPayloadField payload;
        
    public:
        MPEGTS(DataPacket*);
        virtual ~MPEGTS();
        
        void info();
/*        
        bool hasError();
        bool isStartPayload();
        bool isPriority();
        bool hasAdaptation();
        bool hasPayload();
        
        t_byte getSyncByte();
        t_byte getScrambleMode();
        t_byte getContinuityCounter();
        
        t_byte* getPID();
        t_byte* getAdaptation();
        t_byte* getPayload();
*/        
        static int size() {
            return TS_PACKET_SIZE;
        }
};
#endif