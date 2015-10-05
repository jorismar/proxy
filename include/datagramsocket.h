#ifndef DATAGRAM_SOCKET
#define DATAGRAM_SOCKET

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "datapacket.h"

class DatagramSocket {
    private:
        int svr_socket;
        int svr_port;
        std::string svr_address;

        struct sockaddr_in svr_addr;
        struct sockaddr_in to_addr;
        struct sockaddr_in from_addr;
        
        socklen_t to_addrlen;
        socklen_t from_addrlen;
        
        DataPacket* packet;
        
    public:
        DatagramSocket(int);
        DatagramSocket(std::string, int);
        virtual ~DatagramSocket();
        
        int Bind();
        int receive(DataPacket*);
        int send(DataPacket*, std::string, int);
        int sendtoclient(DataPacket*);
        int reply(DataPacket*);
        void Close();

        int getPort();
        std::string getHostAddress();
        void registerclient(std::string, int);
};

#endif
