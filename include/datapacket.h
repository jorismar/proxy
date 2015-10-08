#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <cstdlib>
#include "types.h"

#define SZE 2908

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
        
        unsigned int copy(DataPacket*, int);
        
        unsigned int size();
};

#endif
