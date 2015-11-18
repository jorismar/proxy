#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "datapacket.h"
#include "types.h"

class Socket {
    private:
        t_socket svr_socket;
        int cl_socket;
        int port;
        
        socklen_t cl_socket_len;
        
        struct sockaddr_in svr_addr;
        
    public:
        Socket(int port);
        virtual ~Socket();
        
        int Bind();
        int Listen(int);
        t_socket Accept();
        int Read(DataPacket*);
        int Send(DataPacket*);
        void Close();
        
        static int readFrom(t_socket orig, DataPacket * data) {
            return read(orig, data->get(), data->size());
        }
        
        static int sendTo(t_socket dest, DataPacket * data) {
            return send(dest, data->get(), data->size(), 0);
        }
        
        static void Close(t_socket socket) {
            close(socket);
        }
};

#endif