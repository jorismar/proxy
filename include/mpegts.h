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
    bool   pload_start;
    bool   trans_prior;
    int    pid;

    // 4º Byte
    t_byte scramb_control;
    bool   adapt_control;
    bool   pload_control;
    t_byte contin_counter;
}tHeader;

typedef struct {
    t_byte length;
    bool discontinuity;
    bool random_access;
    bool es_priority;
    bool pcr_flag;
    bool opcr_flag;
    bool splic_flag;
    bool private_data;
    bool extension;
    t_byte pcr[6];
    t_byte opcr[6];
    t_byte splic_count;
    t_byte * stuff;
} tAdaptationField;

class MPEGTS {
    private:
        tHeader header;
        tAdaptationField adapt;
        
    public:
        MPEGTS(DataPacket*);
        virtual ~MPEGTS();
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