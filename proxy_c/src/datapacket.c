#include "datapacket.h"

tDataPacket newPacket(int size) {
    tDataPacket packet;
    
    packet.size = size;
    packet.data = (byte*) malloc(sizeof(byte) * size);
    
    packet.getData = &get_data;
    packet.copyByRange = &copy_by_range;
    packet.length = &length;
    
    return packet;
}

byte * get_data() {
    return 
}

unsigned int copy_by_range(tDataPacket*, unsigned int, unsigned int) {
    
}

unsigned int length() {
    
}
