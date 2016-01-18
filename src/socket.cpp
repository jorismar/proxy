#include "socket.h"

Socket::Socket() {
    this->svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(svr_socket < 0)
        perror("ERROR on open socket");
}

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
    return this->Bind(this->port);
}

int Socket::Bind(int port) {
    int r;
    
    memset((char*) &this->svr_addr, 0, sizeof(this->svr_addr));
    
    this->svr_addr.sin_family = AF_INET;
    this->svr_addr.sin_addr.s_addr = INADDR_ANY;
    this->svr_addr.sin_port = htons(port);
    
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

int Socket::Connect(std::string ip, int port) {
    int r;
    
    this->svr_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    this->svr_addr.sin_family = AF_INET;
    this->svr_addr.sin_port = htons(port);
    
    r = connect(this->svr_socket, (struct sockaddr*) &this->svr_addr, sizeof(this->svr_addr));
    
    if(r < 0)
        perror("ERROR on connect");
    
    return r;
}

int Socket::Read(t_byte * data, t_size length) {
    return read(this->cl_socket, data, length);
}

int Socket::Send(t_byte * data, t_size length) {
    return send(this->svr_socket, data, length, 0);
}

int Socket::Response(t_byte * data, t_size length) {
    return send(this->cl_socket, data, length, 0);
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
