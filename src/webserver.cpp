#include "webserver.h"

Webserver::Webserver(int svr_port, int dash_profile, Buffer **video_buffer, Buffer **audio_buffer, std::string path, std::string dash_path, bool is_sequential) {
    this->port = svr_port;
    this->dash_profile = dash_profile;
    this->dash_path = dash_path;
    this->v_dash_buffer = video_buffer;
    this->a_dash_buffer = audio_buffer;
    this->page_path = path;
}

Webserver::~Webserver() {
    this->alive = false;
    this->socket->~Socket();
}

bool Webserver::openConnection() {
    bool r = true;
    
    this->socket = new Socket(this->port);
    if(this->socket->Bind() < 0) {
        r = false;
        PRINT("\n[ERROR] Webserver bind error");
    }else if(this->socket->Listen(10) < 0) {
        r = false;
        PRINT("\n[ERROR] Webserver listen error");
    }
    
    return r;
}

void Webserver::start() {
    t_socket client;
    
    this->alive = true;

    while(alive) {
        client = this->socket->Accept();
        std::thread cl([=](){Webserver::startClient(client); return 1;});
        cl.detach();
    }
    
    PRINT("[INFO] Session closed!")

    this->socket->Close();
}

void Webserver::stop() {
    this->alive = false;
}

void Webserver::startClient(t_socket cl) {
    VirtualFile * file;
    std::string hder;
    t_size filesize;
    t_byte * rcv_packet = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
    t_byte * snd_packet;
    t_size headsize;
    Http * header = new Http();
    
    do {
        if(Socket::readFrom(cl, rcv_packet, Http::BufferSize::MAX, 5) < 1)
           break;

        header->processRequest(rcv_packet);
        
        file = this->getFile(header->get_reqsted_file());
        
        if(file != NULL) {
            filesize = file->size();
            hder = header->genResponse(filesize, file->filetype(), file->filemodified_date(), Http::Status::OK);
        } else {
            filesize = 0;
            hder = header->genResponse(0, "", "", Http::Status::NOT_FOUND);
        }
        
        headsize = hder.length();
        
        snd_packet = (t_byte*) malloc(sizeof(t_byte) * (headsize + filesize));
        
        memcpy(snd_packet, hder.c_str(), headsize);
        
        if(file != NULL)
            memcpy(snd_packet + headsize, file->binary() + header->get_start_range_pos(), filesize - header->get_start_range_pos());
        
        if(Socket::sendTo(cl, snd_packet, headsize + filesize) < 1)
            break;
        
        header->clear();
        free(snd_packet);
        memset(rcv_packet, 0, Http::BufferSize::MAX);
    } while(file != NULL); //while keep-alive
    
    free(rcv_packet);
        
    Socket::Close(cl);
}

VirtualFile * Webserver::getFile(std::string filename) {
    VirtualFile * file = NULL;
    std::string rqstfile;
	
	int type;
	std::string filetype = Http::getContentType(filename, &type);
    
    if(type == Http::ContentType::DASH) {
        if(this->dash_profile == Dash::Profile::LIVE) {
            file = this->readExternalBuffer(filename);
            return file;
        } else if(this->dash_profile == Dash::Profile::ON_DEMAND) {
            rqstfile = this->dash_path + filename;
        }
    } else {
        if(!filename.compare("/")) {
            rqstfile = this->page_path + "/index.html";
            filetype = "html";
        } else rqstfile = this->page_path + filename;
    }
    
    file = this->readFile(rqstfile, filetype);

    return file;
}

VirtualFile * Webserver::readFile(std::string path, std::string filetype) {
	int count;
	char * bin;
	FILE * pfile;
    VirtualFile * vfile;
    
	pfile = fopen(path.c_str(), "rb");
	
    if(pfile == NULL) return NULL;

    fseek(pfile, 0, SEEK_END);
    long fsize = ftell(pfile);
    rewind(pfile);
    
    bin = (char*) malloc (sizeof(char) * fsize);

    count = fread(bin, 1, fsize, pfile);
    
    vfile = new VirtualFile(path, filetype, Http::getDate(), bin, count);

    fclose(pfile);
        
    return vfile;
}

VirtualFile * Webserver::readExternalBuffer(std::string filename) {
    VirtualFile * file = NULL;
    Buffer ** buff = filename.find("vid") != std::string::npos ? this->v_dash_buffer : this->a_dash_buffer;
    // Se necessário, inserir comparativo para audio
    
    return (*buff)->next();
}

void Webserver::setPort(int svr_port) {
    this->port = svr_port;
}

//MP4Box -dash 4000 -rap -segment-name -profile live -out video.mpd video.mp4#video video.mp4#audio
//MP4Box -dash 4000 -rap -profile live -out video.mpd video.mp4#video video.mp4#audio
//EXIT_IF(pfile == NULL, "Error: File not found.");
//std::thread cl(&Webserver::startClient, this);
//std::this_thread::sleep_for (std::chrono::milliseconds(1000));