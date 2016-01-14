#include "session.h"

Session::Session(std::string id, int udp_port, t_size buffer_size, int http_port, std::string path) {
    this->id = id;
    this->path = path;
    this->udp_port = udp_port;
    this->http_port = http_port;
    this->video_dash_buffer = new Buffer(buffer_size);
    this->audio_dash_buffer = new Buffer(buffer_size - 1);
//  this->dashserver = new Dashcast(udp_port, &this->dashbuffer);
    this->webserver = new Webserver(http_port, &this->video_dash_buffer, &this->audio_dash_buffer, path, true);
}

Session::~Session() {
    this->video_dash_buffer->~Buffer();
    this->audio_dash_buffer->~Buffer();
//  this->dashserver->~Dashcast();
    this->webserver->~Webserver();
}

bool Session::bindUdpPort() {
    /* Bind UDP Dash Port retorna falso se a porta não foi aberta */
    return true;
}

bool Session::bindHttpPort() {
    return this->webserver->openConnection();
}

/*************************************** TESTE *****************************************/
void readfl(std::string filename, std::string path, Buffer** buffer) {
	int count;
	char * bin;
	t_size fsize;
	FILE * pfile;
	VirtualFile * vfile;
	
	for(int i = 1; i < (*buffer)->size(); i++) {
		pfile = fopen((path + filename + std::to_string(i) + ".m4s").c_str(), "rb");

		EXIT_IF(pfile == NULL, "ERRO: Arquivo não encontrado!" << std::endl << (path + filename + std::to_string(i) + ".m4s"));
		
		fseek(pfile, 0, SEEK_END);
		fsize = ftell(pfile);
		rewind(pfile);
		
		bin = (char*) malloc (sizeof(char) * fsize);
	
		count = fread(bin, 1, fsize, pfile);
		
		vfile = new VirtualFile(filename + std::to_string(i) + ".m4s", "m4s", Http::getDate(), bin, count);
		
		EXIT_IF((*buffer)->add(vfile), "ERRO: Falha ao escrever no buffer")
		
		fclose(pfile);
	}
    
    PRINT(filename + " files loaded");
}
/***************************************************************************************/

void Session::start() {
/******************* DASHCAST *******************/
	std::thread vid([=](){readfl("video", this->path + "/fragments/", &this->video_dash_buffer); return 1;});
	std::thread aud([=](){readfl("audio", this->path + "/fragments/", &this->audio_dash_buffer); return 1;});
    
    vid.detach();
    aud.detach();
/************************************************/
	std::thread websvr([=](){this->webserver->start(); return 1;});

    websvr.join();
}

void Session::stop() {
    this->webserver->stop();
//  this->dashserver->stop();
}

std::string Session::getID() {
    return this->id;
}

void Session::setUdpPort(int port) {
//  this->dashserver->setPort(port);
}

void Session::setHttpPort(int port) {
    this->webserver->setPort(port);
}

int Session::getUdpPort() {
    return this->udp_port;
}

int Session::getHttpPort() {
    return this->http_port;
}