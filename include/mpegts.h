#ifndef MPEGTS_H
#define MPEGTS_H

#include "types.h"
#include "datapacket.h"

class MPEGTS {
    private:
        bool trans_err;
        bool payload_unit_start;
        bool is_priority;
        bool has_adaptat_field;             // Adaptation Field Control
        bool has_payload_field;             // Adaptation Field Control

        t_byte sync_byte;
        t_byte trans_scramb_control;
        t_byte continuity_counter;

        t_byte * pid;
        t_byte * adaptation;
        t_byte * payload;
        
    public:
        MPEGTS(DataPacket*);
        virtual ~MPEGTS();
        
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
};
#endif