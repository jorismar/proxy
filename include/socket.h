#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"

class Socket {
    private:
        t_socket svr_socket;
        int cl_socket;
        int port;
        
        socklen_t cl_socket_len;
        
        struct sockaddr_in svr_addr;
        int rcv_timeout;
        
    public:
        Socket();
        Socket(int port);
        virtual ~Socket();
        
        int Bind();
        int Bind(int);
        int Listen(int);
        t_socket Accept();
        int Connect(std::string, int);
        int Read(t_byte*, t_size);
        int Send(t_byte*, t_size);
        int Reply(t_byte*, t_size);
        int Receive(t_byte*, t_size, int);
        void Close();
        void CloseServer();
        void CloseClient();
        int getPort();
        void setPort(int);
        
        static int readFrom(t_socket socket, t_byte * data, t_size size, int timeout) {
            int r;
            struct timeval tv;
            
            if(timeout > 0) {
                tv.tv_sec  = timeout;
                tv.tv_usec = 0;
            
                setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            }
            
            r = recv(socket, data, size, 0);
            
            if(r < 0)
                perror("ERROR on receive");
        
            return r;
        }
        
        static int readFrom(t_socket orig, t_byte * data, t_size size) {
            int r;
        
            r = read(orig, data, size);
        
            if(r < 0)
                perror("ERROR on receive");

            return r;
        }
        
        static int sendTo(t_socket dest, t_byte * data, t_size size) {
            int r;
        
            signal(SIGPIPE, SIG_IGN);
        
            r = send(dest, data, size, 0);
        
            if(r < 0)
                perror("ERROR on send");

            return r;
        }
        
        static void Close(t_socket socket) {
            close(socket);
        }
};

#endif