#ifndef DATAGRAM_SOCKET_H
#define DATAGRAM_SOCKET_H

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"

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
        
    public:
        DatagramSocket(int);
        DatagramSocket(std::string, int);
        virtual ~DatagramSocket();
        
        int Bind();
        int Receive(t_byte*, t_size);
        int SendTo(t_byte*, t_size, std::string, int);
        int Send(t_byte*, t_size);
        int Reply(t_byte*, t_size);
        void Close();

        int getPort();
        std::string getHostAddress();
        void registerclient(std::string, int);
        int getSocket();
};

#endif
