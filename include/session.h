#include <string>
#include "webserver.h"
#include "buffer.h"
#include "types.h"
//#inclide "dashcast.h"

class Session {
    private:
        std::string id;
        std::string path;
        Webserver * webserver;
        Buffer * video_dash_buffer;
        Buffer * audio_dash_buffer;
    //  Dashcast * dashserver;

    
    public:
        Session(std::string, int, t_size, int, std::string);
        virtual ~Session();
        
        bool bindUdpPort();
        bool bindHttpPort();
        
        void start();
        void stop();
        
        std::string getID();
        void setUdpPort(int);
        void setHttpPort(int);
};