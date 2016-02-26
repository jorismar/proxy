#include "dashserver.h"

/***************************************************************************************/

DashServer::DashServer(std::string ip, int port, std::string path, std::string mpd, int profile, bool onthefly, Buffer **vid_buffer, Buffer **aud_buffer) {
    this->ip   = ip;    
    this->port = port;    
    this->path = path;    
    this->mpd  = mpd;    
    this->profile = profile;
    this->isonthefly = onthefly;
    this->video_buffer = vid_buffer;
    this->audio_buffer = aud_buffer;
    
/************************* Mirror *************************/
    this->engine_port = port + 1;
    this->alive = false;
/**********************************************************/
}

/***************************************************************************************/

DashServer::~DashServer() {
    this->alive = false;
    this->rcv->Close();
    this->snd->Close();
}

/***************************************************************************************/

bool DashServer::openConnection() {
    this->rcv = new DatagramSocket(this->port);
    this->snd = new DatagramSocket("127.0.0.1", this->engine_port);

    return this->rcv->Bind() < 0;
}

/***************************************************************************************/

void DashServer::start() {
    execute("rm -f -r -d " + this->path);
    //execute("nodejs /usr/local/etc/engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path + " || " + "node /usr/local/etc/engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path);
    
/************************* Mirror *************************/
    this->alive = true;
    t_byte * buffer = (t_byte*) malloc(sizeof(t_byte) * DatagramSocket::MaxPacketSize);
    
    execute("nodejs /usr/local/etc/engine/bin/live-stream udp://:" + std::to_string(this->engine_port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path + " || " + "node /usr/local/etc/engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path);
    
    std::thread mirror([=](){
        do {
            this->snd->Send(buffer, DatagramSocket::MaxPacketSize);
            
            memset((char*) &buffer, 0, DatagramSocket::MaxPacketSize);
            
            this->rcv->Receive(buffer, DatagramSocket::MaxPacketSize);
        } while(this->alive);

        return 0;
    });

    this->rcv->Receive(buffer, DatagramSocket::MaxPacketSize);
    
    mirror.detach();
   
    do {
        // IMPLEMENTAR FUNÇÃO PARA RECUPERAR E ALTERAR O starNumber="n", contador, e escrever arquivo
    } while(this->alive);
    
    
    
/**********************************************************/
   
}

/***************************************************************************************/

void DashServer::stop() {
    this->alive = false;
}

/***************************************************************************************/

void DashServer::setPort(int port) {
    this->port = port;
}

/***************************************************************************************/

int DashServer::getPort() {
    return this->port;
}
