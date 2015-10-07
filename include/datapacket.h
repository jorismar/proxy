#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <cstdlib>
#include "types.h"

class DataPacket {
    private:
        unsigned int sz;
        t_byte * data;
        
    public:
        DataPacket(unsigned int);
        DataPacket(DataPacket*);
        virtual ~DataPacket();
        
        t_byte* get();
        void set(t_byte*);
        
        unsigned int size();
};

#endif
