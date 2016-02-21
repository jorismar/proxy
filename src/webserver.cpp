#include "webserver.h"

Webserver::Webserver(int svr_port, bool is_on_the_fly, Buffer **video_buffer, Buffer **audio_buffer, std::string path, std::string dash_path) {
    this->port = svr_port;
    this->dash_path = dash_path;
    this->v_dash_buffer = video_buffer;
    this->a_dash_buffer = audio_buffer;
    this->page_path = path;
    this->on_the_fly = is_on_the_fly;
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
        
        if(client >= 0) {
            std::thread cl([=](){Webserver::startClient(client); return 1;});
            cl.detach();
        }
    }
    
    PRINT("[INFO] Session closed!")

    this->socket->CloseServer();
}

void Webserver::stop() {
    this->alive = false;
}

void Webserver::startClient(t_socket cl) {
    VirtualFile * file;
    t_byte * buffer = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
    Http * header = new Http();
    
    //do {
        /*if(*/Socket::readFrom(cl, buffer, Http::BufferSize::MAX, 5)/* < 1)*/;
           //break;

        header->processRequest(buffer);
        
        file = this->getFile(header->getReqstedFile());
        
        if(file != NULL) {
            header->createResponseHeader(file->size(), file->filetype(), Http::Status::OK);
            header->createBinaryPacket(file->binary(), file->size());
        } else {
            header->createResponseHeader(0, "", Http::Status::NOT_FOUND);
            header->createBinaryPacket(NULL, 0);
        }
        
        /*if(*/Socket::sendTo(cl, header->getBinaryPacket(), header->getBinarySize())/* < 1)*/;
            //break;
        if(file != NULL)
            file->~VirtualFile();
        
        header->~Http();
        //memset(buffer, 0, Http::BufferSize::MAX);
    //} while(file != NULL); //while keep-alive
    
    free(buffer);
        
    Socket::Close(cl);
}

VirtualFile * Webserver::getFile(std::string filename) {
    VirtualFile * file = NULL;
    std::string rqstfile;
	
	int type;
	std::string filetype = Http::filename2ContentType(filename, &type);
    
    if(type == Http::ContentType::DASH) {
        if(this->on_the_fly) { // Implement to ignore Initializations and MPD
            file = this->readExternalBuffer(filename);
            return file;
        } else /*if(this->dash_profile == Dash::Profile::ON_DEMAND)*/ {
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
    
    vfile = new VirtualFile(path, filetype, "", bin, count);

    fclose(pfile);
    
    free(bin);

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

int Webserver::getPort() {
    return this->port;
}

//MP4Box -dash 4000 -rap -segment-name -profile live -out video.mpd video.mp4#video video.mp4#audio
//MP4Box -dash 4000 -rap -profile live -out video.mpd video.mp4#video video.mp4#audio
//EXIT_IF(pfile == NULL, "Error: File not found.");
//std::thread cl(&Webserver::startClient, this);
//std::this_thread::sleep_for (std::chrono::milliseconds(1000));