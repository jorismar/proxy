#include "webserver.h"

Webserver::Webserver(std::string svr_name, int svr_port, Buffer * buffr) {
    this->protocol = new Http(svr_name);
    this->socket = new Socket(svr_port);
    this->buffer = buffr;
}

Webserver::~Webserver() {
    // NOT IMPLEMENTED
}

void Webserver::start() {
    this->alive = true;

    this->socket->Bind();
    this->socket->Listen(10);
    
    while(alive) {
        PRINT("Waiting for connection...");
        this->client = this->socket->Accept();
        std::thread cl(&Webserver::startClient, this);
        cl.join();
    }
    
    this->socket->Close();
}

void Webserver::stop() {
    
}

void Webserver::startClient() {
    t_socket skt = this->client;
    DataPacket * msg = new DataPacket(1500);
    
    Socket::readFrom(skt, msg);
    this->protocol->read_msg(msg);
    
    Socket::Close(skt);
    this->alive = false;
}
