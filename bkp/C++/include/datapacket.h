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
        virtual ~DataPacket();
        
        t_byte* get();
        void set(t_byte*);
        
        t_size copy(DataPacket*, int);
        
        t_size size();
};

#endif
