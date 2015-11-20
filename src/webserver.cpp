#include "webserver.h"

Webserver::Webserver(std::string svr_name, int svr_port, Buffer * buffr) {
    this->protocol = new Http(svr_name);
    this->socket = new Socket(svr_port);
    this->buffer = buffr;
}

Webserver::~Webserver() {
    // NOT IMPLEMENTED
}

void Webserver::start() {
    this->alive = true;

    this->socket->Bind();
    this->socket->Listen(10);
    
    while(alive) {
        PRINT("Waiting for connection...");
        this->client = this->socket->Accept();
        std::thread cl(&Webserver::startClient, this);
        cl.join();
    }
    
    this->socket->Close();
}

void Webserver::stop() {
    
}

void Webserver::startClient() {
    t_socket skt = this->client;
    DataPacket * msg = new DataPacket(1500);
    std::string str = "HTTP/1.0 200 OK\nDate: Thu, 19 Nov 2015 19:55:58 GMT\nConnection: close\nCache-control: private Content-type: video/ogg\nServer: MyServer";
    Buffer * fbuffer = new Buffer(1000, 1025);
    t_byte fbin[1025];
    DataPacket * aux = new DataPacket(1025);
    t_socket file = open("/home/jorismar/node_modules/test-engine-live-tools/site/video.ogg",O_RDONLY);
    int cnt, xcnt = 0, finalcnt;
    
    Socket::readFrom(skt, msg);
    //this->protocol->read_msg(msg);

    msg->set((t_byte*) str.c_str());
    
    Socket::sendTo(skt, msg);
    
    PRINT("Sending video stream...");

    while(true){
        std::this_thread::sleep_for (std::chrono::milliseconds(400));
        cnt = Socket::readFrom(file, aux);

        if(xcnt==7)   
            break;   
        else if(xcnt > 0 && cnt != 0)   
            xcnt = 0;   
        else if(xcnt >=0 && cnt == 0){   
            sleep(1);   
            xcnt++;   
            continue;   
        }   
    
        finalcnt = Socket::sendTo(skt, aux->get(), cnt);
        //sleep(1);
        //printf("Data written 1 = %d bytes: %s\n",finalcnt,strerror(errno));

        if(finalcnt <= 0){   
            memcpy(fbin, aux->get(), cnt);  
            finalcnt = cnt;
            PRINT("REDEFINIDA!");
            break;   
        }   
    }   
    
    if(xcnt == 7) return;
/*    
    //    
    
    
    if(fbuffer->next() == NULL) PRINT("OK_OK");
    
    file = fopen("/home/jorismar/node_modules/test-engine-live-tools/site/video.ogg", "rb");

    t_size fsize = ftell(file);
    rewind(file);
    
    fbin = (t_byte*) malloc(sizeof(t_byte) * fsize);
    
    EXIT_IF(file == NULL, "ERRO while open file");
    
    //PRINT("Loading video file...");
    //while (!feof(file) && (fgets(fbin, 1025, file) != NULL)) {
    //    fbuffer->add(new DataPacket(fbin, 1025));
    //}
    
    Socket::readFrom(skt, msg);
    this->protocol->read_msg(msg);
    
    msg->set((t_byte*) str.c_str());
    
    Socket::sendTo(skt, msg);
    
    PRINT("Sending video stream...");
    //do {
    while (!feof(file) && (fread(fbin, 1, fsize, file) == fsize)) {
        Socket::sendTo(skt, new DataPacket(fbin, 1025));
        
        PRINT("Enviado!");
    }
    //} while(aux != NULL);
    
    fclose (file);

    //Socket::sendTo(skt, msg);
*/    
    Socket::Close(skt);
    //this->alive = false;
}
