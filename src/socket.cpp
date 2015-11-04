#include "socket.h"

Socket::Socket(int port) {
    this->svr_socket = socket(AF_INET,SOCK_STREAM,0);
    
    EXIT_IF(svr_socket < 0, "ERROR: Failed to open http socket.");
    
    this->port = port;
}

Socket::~Socket() {
    
}

int Socket::Bind() {
    int r;
    
    memset((char*) &this->svr_addr, 0, sizeof(this->svr_addr));
    this->svr_addr.sin_family = AF_INET;
    this->svr_addr.sin_addr.s_addr = INADDR_ANY;
    this->svr_addr.sin_sin_port = htons(this->port);
    
    r = bind(this->svr_socket, (struct sockaddr*) &this->svr_addr, sizeof(this->svr_addr));
        
    EXIT_IF(r < 0, "ERROR on binding");
    
    return ret;
}

int Socket::Listen(int backlog) {
    int r;
    
    r = listen(this->svr_socket, backlog);
    
    EXIT_IF(r < 0, "ERROR on listening");
    
    return r;
}

void Socket::Accept() {
    this->cl_socket = accept(this->svr_socket, (struct sockaddr*) &this->cl_socket, &this->cl_socket_len);
    
    EXIT_IF(this->cl_socket < 0, "ERROR on accept");
}

int Socket::Read(DataPacket * data) {
    return read(this->cl_socket, data->get(), data->size());
}

int Socket::Send(DataPacket * data) {
    return send(this->cl_socket, data->get(), data->size(), 0);
}

void Socket::Close() {
    close(this->svr_socket);
    close(this->cl_socket);
}
