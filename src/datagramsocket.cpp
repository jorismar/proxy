#include "datagramsocket.h"

DatagramSocket::DatagramSocket(int port) {
    this->svr_socket    = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(svr_socket < 0)
        perror("ERROR on open socket");
    
    this->svr_address   = "";
    this->svr_port      = port;
    this->from_addrlen  = sizeof(from_addr);
}

DatagramSocket::DatagramSocket(std::string address, int port) {
    this->svr_socket    = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(svr_socket < 0)
        perror("ERROR on open socket");
    
    this->svr_address   = address;
    this->svr_port      = port;
    this->from_addrlen  = sizeof(from_addr);
}

DatagramSocket::~DatagramSocket() {
    close(this->svr_socket);
}

int DatagramSocket::Bind() {
    int r;
    
    //bzero((char*) &this->svr_addr, sizeof(this->svr_addr));
    memset((char*) &this->svr_addr, 0, sizeof(this->svr_addr));

    this->svr_addr.sin_family = AF_INET;//AF_UNSPEC
    this->svr_addr.sin_port = htons(this->svr_port);
    
    if(this->svr_address.length() == 0)
        this->svr_addr.sin_addr.s_addr = INADDR_ANY; // localhost server
    else 
        inet_pton(AF_INET, this->svr_address.c_str(), &this->svr_addr.sin_addr); // remote server
 
    r = bind(this->svr_socket, (struct sockaddr*) &this->svr_addr, sizeof(this->svr_addr));
    
    if(r < 0)
        perror("ERROR on binding");
    
    return r;
}

int DatagramSocket::Receive(t_byte* data, t_size length) {
    int r;
    
    r = recvfrom(this->svr_socket, data, length, 0, (struct sockaddr *) &this->from_addr, &this->from_addrlen);
    
    if(r < 0)
        perror("ERROR on receive");
        
    return r;
}

int DatagramSocket::SendTo(t_byte* data, t_size length, std::string address, int port) {
    struct sockaddr_in toaddr;
    int r, toaddrlen = sizeof(toaddr);
    
    memset((char*) &toaddr, 0, toaddrlen);

    toaddr.sin_family = AF_INET;//AF_UNSPEC;
    inet_pton(AF_INET, address.c_str(), &toaddr.sin_addr);
    toaddr.sin_port = htons(port);

    r = sendto(this->svr_socket, data, length, 0, (struct sockaddr *) &toaddr, toaddrlen);
    
    if(r < 0)
        perror("ERROR on send");
        
    return r;
}

int DatagramSocket::Send(t_byte* data, t_size length) {
    int r;
    
    r = sendto(this->svr_socket, data, length, 0, (struct sockaddr *) &this->to_addr, this->to_addrlen);
    
    if(r < 0)
        perror("ERROR on send to client");
        
    return r;
}

int DatagramSocket::Reply(t_byte* data, t_size length) {
    int r;
    
    r = sendto(this->svr_socket, data, length, 0, (struct sockaddr *) &from_addr, from_addrlen);
    
    if(r < 0)
        perror("ERROR on reply");
        
    return r;
}

void DatagramSocket::Close() {
    close(this->svr_socket);
}

int DatagramSocket::getPort() {
    return this->svr_port;
}

std::string DatagramSocket::getHostAddress() {
    return this->svr_address;
}

void DatagramSocket::registerclient(std::string address, int port) {
    this->to_addrlen = sizeof(this->to_addr);
    
    memset((char*) &this->to_addr, 0, this->to_addrlen);

    this->to_addr.sin_family = AF_INET;//AF_UNSPEC;
    inet_pton(AF_INET, address.c_str(), &this->to_addr.sin_addr);
    this->to_addr.sin_port = htons(port);
}

int DatagramSocket::getSocket() {
    return this->svr_socket;
}
