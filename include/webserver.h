#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <cstdio>
#include <cstddef>

#include "datapacket.h"
#include "buffer.h"
#include "socket.h"
#include "types.h"
#include "virtualfile.h"
#include "http.h"

class Webserver {
    private:
        Http * header;
        Socket * socket;
        Buffer * page_buffer;
        Buffer ** dash_buffer;
        t_socket client;
        std::string page_path;
        bool alive;
        
    public:
        Webserver(int, Buffer**, std::string);
        virtual ~Webserver();
        
        void start();
        void stop();
        
        VirtualFile* getFile(Http*);
        VirtualFile* openFile(std::string);
        void startClient(t_socket);
};

#endif

/*
http://www.anirudhtom.com/2011/03/live-streaming-video-tutorial-for.html
http://download.tsi.telecom-paristech.fr/gpac/doc/libgpac/mpegts_8h.html#structtag__m2ts__es
https://pt.wikipedia.org/wiki/Signal.h
http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
http://www.gdsw.at/languages/c/programming-bbrown/c_075.htm
http://www.cplusplus.com/reference/cstdio/setbuf/
http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
http://www.cplusplus.com/reference/ctime/


*/