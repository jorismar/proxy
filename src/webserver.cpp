#include "webserver.h"

Webserver::Webserver(int svr_port, Buffer **filebuffer) {
    this->header = new Http();
    this->socket = new Socket(svr_port);
    this->fbuffer = filebuffer;
}

Webserver::~Webserver() {
    // NOT IMPLEMENTED
}

void Webserver::start() {
    this->alive = true;

    this->socket->Bind();
    this->socket->Listen(10);
    PRINT("Server started!")
    while(alive) {
        //PRINT("Waiting for connection...");
        this->client = this->socket->Accept();
        std::thread cl(&Webserver::startClient, this);
        cl.detach();
    }
    
    this->socket->Close();
}

void Webserver::stop() {
    this->alive = false;
}

void Webserver::startClient() {
    std::string hder;
    t_socket skt = this->client;
    VirtualFile * file;
    char * buff;
    DataPacket * packet = new DataPacket(1024);
    
    Socket::readFrom(skt, packet);
    this->header->process(packet);
    
    PRINT("\nHeader received:\n" << packet->get());

    file = (*fbuffer)->get(0); //->next();
    
    hder = this->header->generate(file->size(), file->filetype(), file->filemodified_date());
    
    PRINT("Header sended:\n" << hder);
    
    buff = (t_byte*) malloc(sizeof(t_byte) * (hder.length() + file->size()));
    
    memcpy(buff, hder.c_str(), hder.length());
    memcpy(buff + hder.length(), file->binary(), file->size());
    
    Socket::sendTo(skt, buff, hder.length() + file->size());
    PRINT("Sending file completed!" << file->filename());

    //std::this_thread::sleep_for (std::chrono::milliseconds(1000));

    Socket::Close(skt);
}


/*
terminate called after throwing an instance of 'std::out_of_range'
  what():  basic_string::at: __n (which is 3) >= this->size() (which is 2)
Aborted (core dumped)
*/