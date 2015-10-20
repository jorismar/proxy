#include "datapacket.h"

DataPacket new_packet(int length) {
    DataPacket packet;
    
    packet.length = length;
    packet.data = (byte*) malloc(sizeof(byte) * length);
    
    return packet;
}

DataPacket new_copy_by_range(const DataPacket * from, unsigned int from_start_pos, unsigned int length) {
    int i, j;
    DataPacket to;
    
    to = new_packet(length);
    
    for(i = 0, j = from_start_pos; i < to.length && j < from->length; i++, j++)
        to.data[i] = from->data[j];
        
    return to;
}

