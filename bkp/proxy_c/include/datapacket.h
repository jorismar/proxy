#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "types.h"

typedef struct tDataPacket{
    unsigned int length;
    byte * data;
} DataPacket;

DataPacket new_packet(int);
DataPacket new_copy_by_range(const DataPacket*, unsigned int, unsigned int);

#endif