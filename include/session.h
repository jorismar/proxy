#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <cstdlib>
#include "webserver.h"
#include "buffer.h"
#include "util.h"
#include "dash.h"

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
        bool on_the_fly;
        int dash_profile;
    
    public:
        Session(std::string, int, int, std::string, int, std::string, std::string, bool, t_size);
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
};

#endif