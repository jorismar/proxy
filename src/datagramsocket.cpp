#include "datagramsocket.h"

DatagramSocket::DatagramSocket(int port) {
    this->svr_socket    = socket(AF_INET, SOCK_DGRAM, 0);
    
    EXIT_IF(svr_socket < 0, "ERROR: Failed to open socket.");
    
    this->svr_address   = "";
    this->svr_port      = port;
    this->from_addrlen  = sizeof(from_addr);
}

DatagramSocket::DatagramSocket(std::string address, int port) {
    this->svr_socket    = socket(AF_INET, SOCK_DGRAM, 0);
    
    EXIT_IF(svr_socket < 0, "ERROR: Failed to open socket.");
    
    this->svr_address   = address;
    this->svr_port      = port;
    this->from_addrlen  = sizeof(from_addr);
}

DatagramSocket::~DatagramSocket() {
    // VAZIO
}

int DatagramSocket::Bind() {
    int ret;
    
    //bzero((char*) &this->svr_addr, sizeof(this->svr_addr));
    memset((char*) &this->svr_addr, 0, sizeof(this->svr_addr));

    this->svr_addr.sin_family = AF_INET;//AF_UNSPEC
    this->svr_addr.sin_port = htons(this->svr_port);
    
    if(strlen(this->svr_address.c_str()) == 0)
        this->svr_addr.sin_addr.s_addr = INADDR_ANY; // localhost server
    else 
        inet_pton(AF_INET, this->svr_address.c_str(), &this->svr_addr.sin_addr); // remote server
 
    ret = bind(this->svr_socket, (struct sockaddr*) &this->svr_addr, sizeof(this->svr_addr));
    
    EXIT_IF(ret < 0, "ERROR on binding");
    
    return ret;
}

int DatagramSocket::receive(DataPacket* packet) {
    return recvfrom(this->svr_socket, packet->get(), packet->size(), 0, (struct sockaddr *) &this->from_addr, &this->from_addrlen);
}

int DatagramSocket::send(DataPacket* packet, std::string address, int port) {
    struct sockaddr_in toaddr;
    int toaddrlen = sizeof(toaddr);
    
    memset((char*) &toaddr, 0, toaddrlen);

    toaddr.sin_family = AF_INET;//AF_UNSPEC;
    inet_pton(AF_INET, address.c_str(), &toaddr.sin_addr);
    toaddr.sin_port = htons(port);

    return sendto(this->svr_socket, packet->get(), packet->size(), 0, (struct sockaddr *) &toaddr, toaddrlen);
}

int DatagramSocket::sendtoclient(DataPacket * packet) {
    return sendto(this->svr_socket, packet->get(), packet->size(), 0, (struct sockaddr *) &this->to_addr, this->to_addrlen);
}

int DatagramSocket::reply(DataPacket* packet) {
    return sendto(this->svr_socket, packet->get(), packet->size(), 0, (struct sockaddr *) &from_addr, from_addrlen);
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
