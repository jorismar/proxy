#include "session.h"

/***************************************************************************************/

Session::Session(std::string id, std::string ip, int udp_port, int http_port, std::string site_path, int dash_profile, std::string dash_path, std::string mpd, bool is_on_the_fly, t_size buffer_size) {
    this->id = id;
	this->ip = ip;
    this->path = site_path;
    this->dash_path = dash_path;
	this->mpd_name = mpd;
    this->udp_port = udp_port;
    this->http_port = http_port;
    this->on_the_fly = is_on_the_fly;
    this->dash_profile = dash_profile;
    
    if(!on_the_fly) {
        this->webserver = new Webserver(http_port, is_on_the_fly, NULL, NULL, path, dash_path);
    } else {
        this->video_dash_buffer = new Buffer(buffer_size);
        this->audio_dash_buffer = new Buffer(buffer_size - 1);
        this->webserver = new Webserver(http_port, is_on_the_fly, &this->video_dash_buffer, &this->audio_dash_buffer, path, dash_path);
    }
}

/***************************************************************************************/

Session::~Session() {
    this->video_dash_buffer->~Buffer();
    this->audio_dash_buffer->~Buffer();
    this->webserver->~Webserver();
}

/***************************************************************************************/

bool Session::bindUdpPort() {
    // Not Implemented Yet
    return true;
}

/***************************************************************************************/

bool Session::bindHttpPort() {
    return this->webserver->openConnection();
}

/***************************************************************************************/

void Session::start() {
    PRINT("[INFO] Session id:" << this->id << " running on UDP:" << this->udp_port << "/HTTP:" << this->http_port);

    std::string cmd = "nodejs ./dash-engine/bin/live-stream udp://" + this->ip + ":" + std::to_string(this->udp_port) + "?fifo_size=50000000 -mpd " + this->mpd_name + " -foldersegments " + this->dash_path;

	PRINT(cmd)
	
    std::thread websvr([=](){this->webserver->start(); return 1;});
    std::thread dash([=](){std::system(cmd.c_str()); return 1;});

    dash.detach();
    websvr.join();
}

/***************************************************************************************/

void Session::stop() {
    this->webserver->stop();
}

/***************************************************************************************/

std::string Session::getID() {
    return this->id;
}

/***************************************************************************************/

int Session::getUdpPort() {
    return this->udp_port;
}

/***************************************************************************************/

int Session::getHttpPort() {
    return this->http_port;
}

/***************************************************************************************/

void Session::setUdpPort(int port) {
    this->udp_port = port;
//  this->dashserver->setPort(port);
}

/***************************************************************************************/

void Session::setHttpPort(int port) {
    this->http_port = port;
    this->webserver->setPort(port);
}
