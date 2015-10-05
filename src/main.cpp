#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include <cstdio>
#include <thread>

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

bool isAlive = false;
Buffer * buffer = new Buffer(BUFFER_SIZE, PACKET_SIZE);


void udpServer() {
    int count = 0;
    int size;
    DataPacket * packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * server = new DatagramSocket(UDP_PORT);
    
    server->Bind();
    
    //server->receive(packet);
    //PRINT("Received: " << packet->get() << std::endl << "Size: " << size);

    server->registerclient("localhost", 1235);
        
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
}

void udpClient() {
    DataPacket * packet = new DataPacket(PACKET_SIZE);
    DatagramSocket * client = new DatagramSocket(UDP_PORT);
    
    client->registerclient("localhost", 1234);
    
    char msg[] = {"TESTE!"};
    
    packet->set(msg);
    client->sendtoclient(packet);
    
    PRINT("Sended!");
}



int main() {
    std::thread svr(udpServer);
    //std::thread clt(udpClient);
    
    svr.join();
    //clt.join();
     return 0;
}

