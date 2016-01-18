#include "webserver.h"

Webserver::Webserver(int svr_port, Buffer **video_buffer, Buffer **audio_buffer, std::string path, bool is_sequential) {
    this->port = svr_port;
    this->v_dash_buffer = video_buffer;
    this->a_dash_buffer = audio_buffer;
    this->page_path = path;
    this->sequential_read_buffer = is_sequential;
    this->current = 0;
}

Webserver::~Webserver() {
    this->alive = false;
    this->socket->~Socket();
}

bool Webserver::openConnection() {
    this->socket = new Socket(this->port);
    return this->socket->Bind() >= 0 && this->socket->Listen(10) >= 0;
}

void Webserver::start() {
    t_socket client;
    
    this->alive = true;

    PRINT("Session running on port: " << this->socket->getPort());
    
    while(alive) {
        client = this->socket->Accept();
        std::thread cl([=](){Webserver::startClient(client); return 1;});
        cl.detach();
    }
    
    PRINT("Session closed!")

    this->socket->Close();
}

void Webserver::stop() {
    this->alive = false;
}

void Webserver::startClient(t_socket cl) {
    VirtualFile * file;
    Http * header = new Http();
    std::string hder;
    t_size filesize = 0;
    t_byte * packet = (t_byte*) malloc(sizeof(t_byte) * 1024);
    
    Socket::readFrom(cl, packet, 1024);
    
    //PRINT(packet)
    
    header->processRequest(packet);

    free(packet);
    
    file = this->getFile(header->get_reqsted_file());
    
    if(file != NULL) {
        hder = header->genResponse(file->size(), file->filetype(), file->filemodified_date());
        filesize = file->size();
    } else hder = header->genResponse(0, "", "");
    
    packet = (t_byte*) malloc(sizeof(t_byte) * (hder.length() + filesize));
    
    memcpy(packet, hder.c_str(), hder.length());
    
    if(file != NULL)
        memcpy(packet + hder.length(), file->binary() + header->get_start_range_pos(), filesize - header->get_start_range_pos());
    
    Socket::sendTo(cl, packet, hder.length() + filesize);
    
    Socket::Close(cl);
}

VirtualFile * Webserver::getFile(std::string filename) {
    int i = 0;
    std::string filetype = filename.substr(filename.rfind(".", filename.length() - 1) + 1, filename.length());
    VirtualFile * file = NULL;
    
    if(!filetype.compare("m4s")) {
        file = this->readExternalBuffer(filename);
    } else {
        if(!filename.compare("/")) {
            filename = "/index.html";
            filetype = "html";
        }
        file = this->readFile(filename, filetype);
    }
    
    return file;
}

VirtualFile * Webserver::readFile(std::string filename, std::string filetype) {
	int count;
	char * bin;
	FILE * pfile;
    std::string path(this->page_path + filename);
    VirtualFile * vfile;

	pfile = fopen(path.c_str(), "rb");
	
    if(pfile == NULL) return NULL;

    fseek(pfile, 0, SEEK_END);
    long fsize = ftell(pfile);
    rewind(pfile);
    
    bin = (char*) malloc (sizeof(char) * fsize);

    count = fread(bin, 1, fsize, pfile);
    
    vfile = new VirtualFile(filename, filetype, Http::getDate(), bin, count);

    fclose(pfile);
        
    return vfile;
}

VirtualFile * Webserver::readExternalBuffer(std::string filename) {
    VirtualFile * file = NULL;
    Buffer ** buff = filename.find("vid") != std::string::npos ? v_dash_buffer : a_dash_buffer;
    // Se necessário, inserir comparativo para audio
    
    if(!this->sequential_read_buffer) {
        do {
            file = (*buff)->get(this->current);
            this->current = ++current % (*buff)->size();
        } while(file != NULL && filename.find(file->filename()) == std::string::npos /*&& i < (*buff)->size()*/);
    } else file = (*buff)->next();
    
    return file;
}

void Webserver::setPort(int port) {
    this->port = port;
}

//MP4Box -dash 4000 -rap -segment-name -profile live -out video.mpd video.mp4#video video.mp4#audio
//MP4Box -dash 4000 -rap -profile live -out video.mpd video.mp4#video video.mp4#audio
//EXIT_IF(pfile == NULL, "Error: File not found.");
//std::thread cl(&Webserver::startClient, this);
//std::this_thread::sleep_for (std::chrono::milliseconds(1000));