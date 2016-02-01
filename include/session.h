#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <cstdlib>
#include "webserver.h"
#include "buffer.h"
#include "types.h"
#include "dash.h"
//#include "dashcast.h"

class Session {
    private:
        std::string id;
        std::string path;
        int udp_port;
        int http_port;
        std::string dash_path;
        std::string mpd_name;
        Webserver * webserver;
        Buffer * video_dash_buffer;
        Buffer * audio_dash_buffer;
    //  Dashcast * dashserver;

    
    public:
        Session(std::string, int, t_size, int, std::string, int, std::string, std::string);
        virtual ~Session();
        
        bool bindUdpPort();
        bool bindHttpPort();
        
        void start();
        void stop();
        
        std::string getID();
        void setUdpPort(int);
        void setHttpPort(int);
        
        int getUdpPort();
        int getHttpPort();
        
        // implementar get ports
};

#endif