#include "mpegts.h"

MPEGTS::MPEGTS(DataPacket * mpegts_packet) {
    t_byte * ts_packet = mpegts_packet->get();
    
    this->header.sync_byte      = ts_packet[0];
    this->header.trans_error    = ts_packet[1] >> 7;
    this->header.pload_start    = ts_packet[1] << 1 >> 7;
    this->header.trans_prior    = ts_packet[1] << 2 >> 7;
    this->header.pid            = ((ts_packet[1] << 3 >> 3) * 256) + ts_packet[2];
    this->header.scramb_control = ts_packet[3] >> 6;
    this->header.adapt_control  = ts_packet[3] << 2 >> 7;
    this->header.pload_control  = ts_packet[3] << 3 >> 7;
    this->header.contin_counter = ts_packet[3] << 4 >> 4;

    PRINT("------ HEADER ------");
    PRINT("SYNC BYTE: " << this->header.sync_byte);
    PRINT("TRANSPORT ERROR: " << (this->header.trans_error ? "YES" : "NO"));
    PRINT("PAYLOAD START: " << (this->header.pload_start ? "YES" : "NO"));
    PRINT("TRANS. PRIORITY: " << (this->header.trans_prior ? "YES" : "NO"));
    PRINT("PID: " << this->header.pid);
    PRINT("SCRAMBLED: " << (this->header.scramb_control == 0 ? "NOT" : this->header.scramb_control == 1 ? "RESERVED" : this->header.scramb_control == 2 ? "EVEN KEY" : "ODD KEY"));
    PRINT("ADAPTATION FIELD: " << (this->header.adapt_control ? "CONTAIN" : "NOT CONTAIN"));
    PRINT("PAYLOAD FIELD: " << (this->header.pload_control ? "CONTAIN" : "NOT CONTAIN"));
    PRINT("CONTINULTY COUNTER: " << (int) this->header.contin_counter);
    
    this->adapt.length          = ts_packet[4];
    int b = this->adapt.length;
    
    PRINT("\n------ ADAPTATION FIELD ------");
    PRINT("LENGTH: " << b);

    if(this->header.adapt_control) {
        this->adapt.discontinuity = ts_packet[5] >> 7;
        this->adapt.random_access = ts_packet[5] << 1 >> 7;
        this->adapt.es_priority   = ts_packet[5] << 2 >> 7;
        this->adapt.pcr_flag      = ts_packet[5] << 3 >> 7;
        this->adapt.opcr_flag     = ts_packet[5] << 4 >> 7;
        this->adapt.splic_flag    = ts_packet[5] << 5 >> 7;
        this->adapt.private_data  = ts_packet[5] << 6 >> 7;
        this->adapt.extension     = ts_packet[5] << 7 >> 7;

        PRINT("DISCONTINUITY: " << (this->adapt.discontinuity ? "YES" : "NO"));
        PRINT("RANDOM ACCESS: " << (this->adapt.random_access ? "YES" : "NO"));
        PRINT("ES PRIORITY: " << (this->adapt.es_priority ? "YES" : "NO"));
        PRINT("PCR: " << (this->adapt.pcr_flag ? "YES" : "NO"));
        PRINT("OPCR: " << (this->adapt.opcr_flag ? "YES" : "NO"));
        PRINT("SPLICING: " << (this->adapt.splic_flag ? "YES" : "NO"));
        PRINT("PRIVATE DATA: " << (this->adapt.private_data ? "YES" : "NO"));
        PRINT("EXTENSION: " << (this->adapt.extension ? "YES" : "NO"));
        PRINT("\n\n");
    }    


          
          


/*        
        if(this->adapt.pcr_flag) {
            this->adapt.pcr[0]    = ts_packet[6];
            this->adapt.pcr[1]    = ts_packet[7];
            this->adapt.pcr[2]    = ts_packet[8];
            this->adapt.pcr[3]    = ts_packet[9];
            this->adapt.pcr[4]    = ts_packet[10];
            this->adapt.pcr[5]    = ts_packet[11];
        }
        
        if(this->adapt.opcr_flag) {
            this->adapt.opcr[0]   = ts_packet[];
            this->adapt.opcr[1]   = ts_packet[];
            this->adapt.opcr[2]   = ts_packet[];
            this->adapt.opcr[3]   = ts_packet[];
            this->adapt.opcr[4]   = ts_packet[];
            this->adapt.opcr[5]   = ts_packet[];
        }
        
        this->adapt.splic_count   = ts_packet[];
        this->adapt.stuff         = 
    }
    
    
    
    
    
    
    
    
    this->sync_byte            
    
    this->trans_err             = 
    this->payload_unit_start    =
    this->trans_priority;       =
    
    this->pid                   = (t_byte*) malloc(sizeof(t_byte) * 2);
    
    *this->pid                  =

    this-> trans_scramb_control = 

    this->adaptat_field_control =
    this->payload_field_control =

    this-> continuity_counter   =
    
    this->adaptat_length        = adaptat_field_control ? ts_packet[4] : 0;

    this->adaptation            = ;
    this->payload;  */
}

MPEGTS::~MPEGTS() {
    
}
/*
bool MPEGTS::hasError() {
    return this->trans_err;
}

bool MPEGTS::isStartPayload() {
    return this->payload_unit_start;
}

bool MPEGTS::isPriority() {
    return this->trans_priority;
}

bool MPEGTS::hasAdaptation() {
    return this->adaptat_field_control;
}

bool MPEGTS::hasPayload() {
    return this->payload_field_control;
}

t_byte MPEGTS::getSyncByte() {
    return this->sync_byte;
}

t_byte MPEGTS::getScrambleMode() {
    return this->trans_scramb_control;
}

t_byte MPEGTS::getContinuityCounter() {
    return this->continuity_counter;
}

t_byte* MPEGTS::getPID() {
    return this->pid;
}

t_byte* MPEGTS::getAdaptation() {
    return this->adaptation;
}

t_byte* MPEGTS::getPayload() {
    return this->payload;
}
*/