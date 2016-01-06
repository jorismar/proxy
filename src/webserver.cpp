#include "webserver.h"

Webserver::Webserver(int svr_port, Buffer **extern_buffer, std::string path) {
    this->socket = new Socket(svr_port);
    //this->page_buffer = new Buffer(500);
    this->dash_buffer = extern_buffer;
    this->page_path = path;
}

Webserver::~Webserver() {
    this->alive = false;
    this->socket->~Socket();
    //this->page_buffer->~Buffer();
}

void Webserver::start() {
    t_socket client;
    
    this->alive = true;

    this->socket->Bind();
    this->socket->Listen(10);
    
    PRINT("Server running on port: " << this->socket->getPort());
    
    while(alive) {
        client = this->socket->Accept();
        std::thread cl([=](){Webserver::startClient(client); return 1;});
        cl.detach();
    }
    
    PRINT("Server stoped!")

    this->socket->Close();
}

void Webserver::stop() {
    this->alive = false;
}

void Webserver::startClient(t_socket cl) {
    std::string hder;
    t_socket skt = cl;
    VirtualFile * file;
    char * buff;
    DataPacket * packet = new DataPacket(1024);
    Http * header = new Http();
    t_size filesize = 0;
    
    Socket::readFrom(skt, packet);
    header->process(packet);
    
    file = this->getFile(header);
    
    if(file != NULL) {
        hder = header->generate(file->size(), file->filetype(), file->filemodified_date());
        filesize = file->size();
    } else hder = header->generate(0, "", "");
    
    buff = (t_byte*) malloc(sizeof(t_byte) * (hder.length() + filesize));
    
    memcpy(buff, hder.c_str(), hder.length());
    
    if(file != NULL)
        memcpy(buff + hder.length(), file->binary() + header->get_start_range_pos(), file->size() - header->get_start_range_pos());
    
    Socket::sendTo(skt, buff, hder.length() + filesize);

    Socket::Close(skt);
}

VirtualFile * Webserver::getFile(Http * header) {
    int i = 0;
    std::string filename = header->get_reqsted_file();
    VirtualFile * file = NULL;
    
    if(!filename.compare("/"))
        filename = "/index.html";
    /** Remover comentário para manter os arquivos da página em memória **/
    
    //do {
    //    file = this->page_buffer->get(i++);
    //} while(file != NULL && file->filename().compare(filename) != 0 && i < this->page_buffer->size());
    
    //if(file == NULL) {
        file = this->openFile(filename);
        
        //if(file != NULL)
            //this->page_buffer->add(file);
    //}
    
    return file;
}

VirtualFile * Webserver::openFile(std::string filename) {
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
    
    vfile = new VirtualFile(filename, filename.substr(filename.rfind(".", filename.length() - 1) + 1, filename.length()), Http::getDate(), bin, fsize);

    fclose(pfile);
        
    return vfile;
}

//EXIT_IF(pfile == NULL, "Error: File not found.");
//std::thread cl(&Webserver::startClient, this);
//std::this_thread::sleep_for (std::chrono::milliseconds(1000));