#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>

class Socket {
    private:
        int svr_socket;
        int cl_socket;
        int port;
        
        int cl_socket_len;
        
        struct sockaddr_in svr_addr;
        
    public:
        Socket(int);
        virtual ~Socket();
        
        int Bind();
        int Listen(int);
        void Accept();
        int Read(DataPacket*);
        int Send(DataPacket*);
        void Close();
};

#endif