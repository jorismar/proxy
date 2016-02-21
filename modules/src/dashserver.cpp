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
}

/***************************************************************************************/

DashServer::~DashServer() {
    // NOT IMPLEMENTED YET
}

/***************************************************************************************/

bool DashServer::openConnection() {
    // NOT IMPLEMENTED YET
    return true;    
}

/***************************************************************************************/

void DashServer::start() {
    execute("rm -f -r -d " + this->path);
    execute("nodejs /usr/local/etc/engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path + " || " + "node /usr/local/etc/engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->port) + "?fifo_size=50000000 -mpd " + this->mpd + " -foldersegments " + this->path);
}

/***************************************************************************************/

void DashServer::stop() {
    // NOT IMPLEMENTED YET
}

/***************************************************************************************/

void DashServer::setPort(int port) {
    this->port = port;
}

/***************************************************************************************/

int DashServer::getPort() {
    return this->port;
}
