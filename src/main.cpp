#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include "webserver.h"
#include <cstdio>
#include <thread>    

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

bool isAlive = false;
Buffer * udp_buffer = new Buffer(BUFFER_SIZE, PACKET_SIZE);
Buffer * ts_buffer = new Buffer(BUFFER_SIZE * 7, 188);

void udpServer() {
    DataPacket * udp_packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * server = new DatagramSocket(UDP_PORT);
    MPEGTS * ts_packet;
    int i = 0;
    
    server->Bind();

    PRINT("Waiting for connection...");
    
    while(true) {
        server->receive(udp_packet);
	while(i < PACKET_SIZE) {
	    ts_packet = new MPEGTS(udp_packet->fragment(i, MPEGTS::size()));
	    i += MPEGTS::size();
	}
	i = 0;
    }
}

int main() {
//    std::thread svr(udpServer);
//    svr.join();

    Webserver * svr = new Webserver("TESTE", 8080, udp_buffer);
    svr->start();

    return 0;
}