#include "session.h"

/***************************************************************************************/

Session::Session(std::string id, std::string ip, int udp_port, int http_port, std::string site_path, int dash_profile, std::string dash_path, std::string mpd, bool is_on_the_fly, t_size buffer_size) {
    this->id = id;
    
    if(is_on_the_fly) {
        this->video_dash_buffer = new Buffer(buffer_size);
        this->audio_dash_buffer = new Buffer(buffer_size - 1);
    } else {
        this->video_dash_buffer = NULL;
        this->audio_dash_buffer = NULL;
    }

    this->webserver = new Webserver(http_port, is_on_the_fly, &this->video_dash_buffer, &this->audio_dash_buffer, site_path, dash_path + "/dash");
    this->dashserver = new DashServer(ip, udp_port, dash_path, mpd, dash_profile, is_on_the_fly, &this->video_dash_buffer, &this->audio_dash_buffer);
}

/***************************************************************************************/

Session::~Session() {
    if(this->video_dash_buffer != NULL)
        this->video_dash_buffer->~Buffer();
        
    if(this->audio_dash_buffer != NULL)
        this->audio_dash_buffer->~Buffer();

    this->webserver->~Webserver();
    this->dashserver->~DashServer();
}

/***************************************************************************************/

bool Session::bindUdpPort() {
    return this->dashserver->openConnection();
}

/***************************************************************************************/

bool Session::bindHttpPort() {
    return this->webserver->openConnection();
}

/***************************************************************************************/

void Session::start() {
    std::thread websvr([=](){this->webserver->start(); return 1;});
    std::thread dashsvr([=](){this->dashserver->start(); return 1;});
    
    PRINT("[INFO] Session id:" << this->id << " running on UDP:" << this->dashserver->getPort() << "/HTTP:" << this->webserver->getPort());

    dashsvr.detach();
    websvr.join();
}

/***************************************************************************************/

void Session::stop() {
    this->webserver->stop();
    this->dashserver->stop();    
}

/***************************************************************************************/

std::string Session::getID() {
    return this->id;
}

/***************************************************************************************/

int Session::getUdpPort() {
    return this->dashserver->getPort();
}

/***************************************************************************************/

int Session::getHttpPort() {
    return this->webserver->getPort();
}

/***************************************************************************************/

void Session::setUdpPort(int port) {
    this->dashserver->setPort(port);
}

/***************************************************************************************/

void Session::setHttpPort(int port) {
    this->webserver->setPort(port);
}
