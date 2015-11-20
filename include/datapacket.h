#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "types.h"

class DataPacket {
    private:
        t_size length;
        t_byte * data;
        
    public:
        DataPacket(t_size);
        DataPacket(DataPacket*);
        DataPacket(t_byte*, t_size);
        virtual ~DataPacket();
        
        t_byte* get();
        void set(t_byte*);
        void setbyte(t_byte, t_pos);
        void printPkt();
        
        t_size size();
        
        DataPacket* fragment(t_pos, t_size);
};

#endif
