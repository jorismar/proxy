#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include <cstdio>
#include <thread>

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

bool isAlive = false;
Buffer * buffer = new Buffer(BUFFER_SIZE, PACKET_SIZE);

void udpServer() {
    int pos = 0;
    //int size;
    //Buffer * ts_buffer = new Buffer(7, 188);
    DataPacket * ts_packet = new DataPacket(MPEGTS::size());
    DataPacket * udp_packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * server = new DatagramSocket(UDP_PORT);
    
    server->Bind();
    
    PRINT("Waiting for connection...");
    server->receive(udp_packet);
    PRINT("Connected!\n\n");
    
    for(int i = 0; i < 7; i++) {
        pos = ts_packet->copy(udp_packet, pos);
        //ts_buffer->add(ts_packet);
        new MPEGTS(ts_packet);
    }
    
    /*
    
    
    
    
    


    //server->registerclient("localhost", 1235);
        
    PRINT("Waiting for packets...");
    
    do {
        server->receive(packet);
        PRINT("Packet received: " << count + 1);
        buffer->set(count, packet);
    } while(++count < BUFFER_SIZE);
    
    count = 0;
    
    do {
        PRINT("Packet sended: " << count + 1);
        server->sendtoclient(buffer->get(count)); 
    } while(++count < BUFFER_SIZE);
*/}

void udpClient() {
    DataPacket * packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * client = new DatagramSocket(UDP_PORT);
    
    client->registerclient("localhost", 1234);
    
    char msg[] = {"TESTE!"};
    
    packet->set(msg);
    client->sendtoclient(packet);
    
    PRINT("Sended!");
}

typedef struct {
    char bt[4];
} teste;


int main() {
    std::thread svr(udpServer);
    //std::thread clt(udpClient);
    
    svr.join();
    //clt.join();
     return 0;
}

