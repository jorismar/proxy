#include "mpegts.h"

MPEGTS::MPEGTS(DataPacket * mpegts_packet) {
    t_byte * ts_packet = mpegts_packet->get();
    
    this->header.sync_byte      = ts_packet[0];
    this->header.trans_error    = ts_packet[1] >> 7;
    this->header.payload_start  = ts_packet[1] << 1 >> 7;
    this->header.trans_prior    = ts_packet[1] << 2 >> 7;
    this->header.pid            = ((ts_packet[1] << 3 >> 3) * 256) + ts_packet[2];
    this->header.scramb_control = ts_packet[3] >> 6;
    this->header.adapt_control  = ts_packet[3] << 2 >> 7;
    this->header.pload_control  = ts_packet[3] << 3 >> 7;
    this->header.contin_counter = ts_packet[3] << 4 >> 4;
    
    this->adaptation.length     = this->header.adapt_control ? ts_packet[4] : 0;

    if(this->header.adapt_control) {
        this->adaptation.discontinuity    = ts_packet[5] >> 7;
        this->adaptation.random_access    = ts_packet[5] << 1 >> 7;
        this->adaptation.es_priority      = ts_packet[5] << 2 >> 7;
        this->adaptation.pcr_flag         = ts_packet[5] << 3 >> 7;
        this->adaptation.opcr_flag        = ts_packet[5] << 4 >> 7;
        this->adaptation.splic_flag       = ts_packet[5] << 5 >> 7;
        this->adaptation.priv_data_flag   = ts_packet[5] << 6 >> 7;
        this->adaptation.extension_flag   = ts_packet[5] << 7 >> 7;
        
        unsigned int i = 6;

        if(this->adaptation.pcr_flag) {
            this->adaptation.pcr.base       = ts_packet[i++];
            this->adaptation.pcr.base       = this->adaptation.pcr.base << 8;
            this->adaptation.pcr.base      += ts_packet[i++];
            this->adaptation.pcr.base       = this->adaptation.pcr.base << 8;
            this->adaptation.pcr.base      += ts_packet[i++];
            this->adaptation.pcr.base       = this->adaptation.pcr.base << 8;
            this->adaptation.pcr.base      += ts_packet[i++];
            this->adaptation.pcr.base       = this->adaptation.pcr.base << 1;
            this->adaptation.pcr.base      += ts_packet[i] >> 7;
            
            this->adaptation.pcr.extension  = ts_packet[i++] << 7 >> 7;
            this->adaptation.pcr.extension  = this->adaptation.pcr.extension << 8;
            this->adaptation.pcr.extension += ts_packet[i++];
        }

        if(this->adaptation.opcr_flag) {
            this->adaptation.opcr.base       = ts_packet[i++];
            this->adaptation.opcr.base       = this->adaptation.opcr.base << 8;
            this->adaptation.opcr.base      += ts_packet[i++];
            this->adaptation.opcr.base       = this->adaptation.opcr.base << 8;
            this->adaptation.opcr.base      += ts_packet[i++];
            this->adaptation.opcr.base       = this->adaptation.opcr.base << 8;
            this->adaptation.opcr.base      += ts_packet[i++];
            this->adaptation.opcr.base       = this->adaptation.opcr.base << 1;
            this->adaptation.opcr.base      += ts_packet[i] >> 7;
            
            this->adaptation.opcr.extension  = ts_packet[i++] << 7 >> 7;
            this->adaptation.opcr.extension  = this->adaptation.opcr.extension << 8;
            this->adaptation.opcr.extension += ts_packet[i++];
        }

        if(this->adaptation.splic_flag) {
            this->adaptation.splice.countdown = ts_packet[i++];
        }

        if(this->adaptation.priv_data_flag) {
            this->adaptation.transp_private.length = ts_packet[i++];
            this->adaptation.transp_private.data = (t_byte*) malloc(sizeof(t_byte) * this->adaptation.transp_private.length);
            
            unsigned int j = i + this->adaptation.transp_private.length;

            while(i < j)
                *this->adaptation.transp_private.data++ = ts_packet[i++];
        }

        if(this->adaptation.extension_flag) {
            this->adaptation.extension.length      = ts_packet[i++];
            this->adaptation.extension.ltw_flag    = ts_packet[i] >> 7;
            this->adaptation.extension.plecew_flag = ts_packet[i] << 1 >> 7;
            this->adaptation.extension.splice_flag = ts_packet[i++] << 2 >> 7;
            
            if(this->adaptation.extension.ltw_flag) {
                this->adaptation.extension.ltw.valid_flag = ts_packet[i] >> 7;
                this->adaptation.extension.ltw.offset     = ts_packet[i++] << 1 >> 1;
                this->adaptation.extension.ltw.offset     = this->adaptation.extension.ltw.offset << 8;
                this->adaptation.extension.ltw.offset    += ts_packet[i++];
            }

            if(this->adaptation.extension.plecew_flag) {
                this->adaptation.extension.plecewise.rate  = ts_packet[i++];
                this->adaptation.extension.plecewise.rate  = this->adaptation.extension.plecewise.rate << 8;
                this->adaptation.extension.plecewise.rate += ts_packet[i++];
                this->adaptation.extension.plecewise.rate  = this->adaptation.extension.plecewise.rate << 8;
                this->adaptation.extension.plecewise.rate += ts_packet[i++];
            }

            if(this->adaptation.extension.splice_flag) {
                this->adaptation.extension.splice.type           = ts_packet[i] >> 4;
                this->adaptation.extension.splice.dts_next_au_1  = ts_packet[i] << 4 >> 5;
                this->adaptation.extension.splice.marker1        = ts_packet[i++] << 7 >> 7;
                this->adaptation.extension.splice.dts_next_au_2  = ts_packet[i++];
                this->adaptation.extension.splice.dts_next_au_2  = this->adaptation.extension.splice.dts_next_au_2 << 7;
                this->adaptation.extension.splice.dts_next_au_2 += ts_packet[i] >> 1;
                this->adaptation.extension.splice.marker2        = ts_packet[i++] << 7 >> 7;
                this->adaptation.extension.splice.dts_next_au_3  = ts_packet[i++];
                this->adaptation.extension.splice.dts_next_au_3  = this->adaptation.extension.splice.dts_next_au_3 << 7;
                this->adaptation.extension.splice.dts_next_au_3 += ts_packet[i] >> 1;
            }
        }
    }
    
    if(this->header.pload_control) {
        int start_pos = this->header.adapt_control ? this->adaptation.length + 5 : 5;
        
        this->payload.length = this->size() - start_pos;
        this->payload.data = new DataPacket(this->payload.length);
        this->payload.data->copy(mpegts_packet, start_pos);
    }
    
    this->info();
}

MPEGTS::~MPEGTS() {
    // EMPTY
}

tHeaderField MPEGTS::getHeader() {
    return this->header;
}

tAdaptationField MPEGTS::getAdaptation() {
    return this->adaptation;
}

tPayloadField MPEGTS::getPayload() {
    return this->payload;
}

void MPEGTS::info() {
    PRINT("------------ HEADER ------------");
    PRINT("SYNC BYTE: " << this->header.sync_byte);
    PRINT("TRANSPORT ERROR: " << (this->header.trans_error ? "YES" : "NO"));
    PRINT("PAYLOAD START: " << (this->header.payload_start ? "YES" : "NO"));
    PRINT("TRANS. PRIORITY: " << (this->header.trans_prior ? "YES" : "NO"));
    PRINT("PID: " << this->header.pid);
    PRINT("SCRAMBLED: " << (this->header.scramb_control == 0 ? "NOT" : this->header.scramb_control == 1 ? "RESERVED" : this->header.scramb_control == 2 ? "EVEN KEY" : "ODD KEY"));
    PRINT("ADAPTATION FIELD: " << (this->header.adapt_control ? "CONTAIN" : "NOT CONTAIN"));
    PRINT("PAYLOAD FIELD: " << (this->header.pload_control ? "CONTAIN" : "NOT CONTAIN"));
    PRINT("CONTINULTY COUNTER: " << (int) this->header.contin_counter);

    if(this->header.adapt_control) {
        PRINT("\n------- ADAPTATION FIELD -------");
        PRINT("LENGTH: " << (int) this->adaptation.length);
        PRINT("DISCONTINUITY: " << (this->adaptation.discontinuity ? "YES" : "NO"));
        PRINT("RANDOM ACCESS: " << (this->adaptation.random_access ? "YES" : "NO"));
        PRINT("ES PRIORITY: " << (this->adaptation.es_priority ? "YES" : "NO"));
        PRINT("PCR FLAG: " << (this->adaptation.pcr_flag ? "YES" : "NO"));
        PRINT("OPCR FLAG: " << (this->adaptation.opcr_flag ? "YES" : "NO"));
        PRINT("SPLICING FLAG: " << (this->adaptation.splic_flag ? "YES" : "NO"));
        PRINT("PRIVATE DATA FLAG: " << (this->adaptation.priv_data_flag ? "YES" : "NO"));
        PRINT("EXTENSION FLAG: " << (this->adaptation.extension_flag ? "YES" : "NO"));

        if(this->adaptation.pcr_flag) {
            PRINT("\n----- ADAPTATION-PCR FIELD -----");
            PRINT("BASE: " << this->adaptation.pcr.base);
            PRINT("EXTENSION: " << this->adaptation.pcr.extension);
        }

        if(this->adaptation.opcr_flag) {
            PRINT("\n---- ADAPTATION-OPCR FIELD -----");
            PRINT("BASE: " << this->adaptation.opcr.base);
            PRINT("EXTENSION: " << this->adaptation.opcr.extension);
        }

        if(this->adaptation.splic_flag) {
            PRINT("\n----- ADAPTATION-SPL FIELD -----");
            PRINT("COUNTDOWN: " << (int) this->adaptation.splice.countdown);
            
        }

        if(this->adaptation.priv_data_flag) {
            PRINT("\n----- ADAPTATION-PVT FIELD -----");
            PRINT("LENGTH: " << (int) this->adaptation.transp_private.length);
            //PRINT("DATA: " << this->adaptation.transp_private.data);
        }

        if(this->adaptation.extension_flag) {
           PRINT("\n----- ADAPTATION-EXT FIELD -----");
           PRINT("LENGTH: " << (int) this->adaptation.extension.length);
           PRINT("LTW FLAG: " << (this->adaptation.extension.ltw_flag ? "YES" : "NO"));
           PRINT("PLECEWISE FLAG: " << (this->adaptation.extension.plecew_flag ? "YES" : "NO"));
           PRINT("SPLICE FLAG: " << (this->adaptation.extension.splice_flag ? "YES" : "NO"));

            if(this->adaptation.extension.ltw_flag) {
                PRINT("\n--- ADAPTATION-EXT-LTW FIELD ---");
                PRINT("VALID FLAG: " << (this->adaptation.extension.ltw.valid_flag ? "YES" : "NO"));
                PRINT("OFFSET: " << (int) this->adaptation.extension.ltw.offset);
            }

            if(this->adaptation.extension.plecew_flag) {
                PRINT("\n--- ADAPTATION-EXT-PLW FIELD ---");
                PRINT("RATE: " << (int) this->adaptation.extension.plecewise.rate);
            }

            if(this->adaptation.extension.splice_flag) {
                PRINT("\n--- ADAPTATION-EXT-SPL FIELD ---");
                PRINT("TYPE: " << this->adaptation.extension.splice.type);
                PRINT("DTS NEXT AU: " << this->adaptation.extension.splice.dts_next_au_1);
                PRINT("MARK: " << this->adaptation.extension.splice.marker1);
                PRINT("DTS NEXT AU: " << this->adaptation.extension.splice.dts_next_au_2);
                PRINT("MARK: " << this->adaptation.extension.splice.marker2);
                PRINT("DTS NEXT AU: " << this->adaptation.extension.splice.dts_next_au_2);
            }
        }
    }
    
    if(this->header.pload_control) {
        PRINT("\n----------- PAYLOAD ------------");
        PRINT("LENGTH: " << (int) this->payload.length);
    }

    PRINT("--------------------------------\n\n");
}

