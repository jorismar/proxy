#include "datapacket.h"

int main() {
    DataPacket packet = new_packet(188);
    
    packet.data[0] = 't';
    packet.data[1] = 'e';
    packet.data[2] = 's';
    packet.data[3] = 't';
    packet.data[4] = 'e';
    packet.data[5] = '\0';

    DataPacket pkt = new_copy_by_range(&packet, 0, packet.length);
    
    printf("SIZE: %i\nDATA: %s\n", pkt.length, pkt.data);
    
    return 0;
}