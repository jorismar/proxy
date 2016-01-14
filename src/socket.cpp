#include "socket.h"

Socket::Socket(int port) {
    this->svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(svr_socket < 0)
        perror("ERROR on open socket");
    
    this->port = port;
}

Socket::~Socket() {
    // NOT IMPLEMENTED
}

int Socket::Bind() {
    int r;
    
    memset((char*) &this->svr_addr, 0, sizeof(this->svr_addr));
    
    this->svr_addr.sin_family = AF_INET;
    this->svr_addr.sin_addr.s_addr = INADDR_ANY;
    this->svr_addr.sin_port = htons(this->port);
    
    r = bind(this->svr_socket, (struct sockaddr*) &this->svr_addr, sizeof(this->svr_addr));
    
    if(r < 0)
        perror("ERROR on binding");
    
    return r;
}

int Socket::Listen(int backlog) {
    int r;
    
    r = listen(this->svr_socket, backlog);
    
    if(r < 0)
        perror("ERROR on listening");
    
    return r;
}

t_socket Socket::Accept() {
    this->cl_socket = accept(this->svr_socket, (struct sockaddr*) &this->cl_socket, &this->cl_socket_len);
    
    if(this->cl_socket < 0)
        perror("ERROR on accept");
    
    return cl_socket;
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

int Socket::getPort() {
    return this->port;
}

int Socket::setPort(int port) {
    this->port = port;
}
