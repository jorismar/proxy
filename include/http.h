#ifndef HTTP_H
#define HTTP_H

// Informational Responses
#define RPLY_CONTINUE               "100"
#define RPLY_SWITCHING              "101"

// Sucessful Responses
#define RPLY_OK                     "200 OK"
#define RPLY_CREATED                "201 Created"
#define RPLY_ACCEPTED               "202 Accepted"
#define RPLY_NON_AUTHORIT_INFO      "203"
#define RPLY_NO_CONTENT             "204"
#define RPLY_RESET_CONTENT          "205"
#define RPLY_PARTIAL_CONTENT        "206 Partial Content"

// Redirection messages
#define RPLY_MULT_CHOICE            "300"
#define RPLY_MOVED_PERMANENTLY      "301"
#define RPLY_FOUND                  "302"
#define RPLY_SEE_OTHER              "303"
#define RPLY_NOT_MODIFIED           "304 Not Modified"
#define RPLY_USE_PROXY              "305"
#define RPLY_TEMP_REDIRECT          "307"
#define RPLY_PERMAN_REDIRECT        "308"

// Client error responses
#define RPLY_BAD_REQUEST            "400 Bad Request"
#define RPLY_UNAUTHORIZED           "401"
#define RPLY_PAYMENT_REQUIRED       "402"
#define RPLY_FORBIDDEN              "403"
#define RPLY_NOT_FOUND              "404 Not Found"
#define RPLY_METHOD_NOT_ALLWED      "405"
#define RPLY_NOT_ACCEPTABLE         "406 Not Acceptable"
#define RPLY_PROXY_AUTHENT_REQD     "407"
#define RPLY_REQUEST_TIMEOUT        "408"
#define RPLY_CONFLICT               "409"
#define RPLY_GONE                   "410"
#define RPLY_LENGTH_REQUIRED        "411"
#define RPLY_PRECONDITION_FAIL      "412"
#define RPLY_PAYLOAD_TOO_LARGE      "413"
#define RPLY_URI_TOO_LONG           "414"
#define RPLY_UNSUPPORT_MEDIA_TYPE   "415"
#define RPLY_REQST_RANGE_NO_SATSF   "416"   
#define RPLY_EXPECTATION_FAILED     "417"
#define RPLY_IM_A_TEAPOT            "418"
#define RPLY_MISDIRECTED_REQST      "421"
#define RPLY_UPGRADE_REQUIRED       "426"
#define RPLY_PRECONDTION_REQRED     "428"
#define RPLY_TOO_MANY_REQUESTS      "429"
#define RPLY_REQST_HEADER_FIELD_TOO_LARGE "431"

// Server error responses
#define RPLY_INTERNAL_SVR_ERROR     "500"
#define RPLY_NOT_IMPLEMENTED        "501 Not Implemented"
#define RPLY_BAD_GATEWAY            "502"
#define RPLY_SERVIC_UNAVAILABLE     "503"
#define RPLY_GATEWAY_TIMEOUT        "504"
#define RPLY_HTTP_VER_NOT_SUPPT     "505"
#define RPLY_VARNT_ALSO_NEGOT       "506"
#define RPLY_VARNT_ALSO_NEGOTIATION "507"
#define RPLY_NETWRK_AUTHENT_REQ     "511"

// Connection
#define CLOSE       "close"
#define KEEP_ALIVE  "keep-alive"

//#define GET 0

#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "util.h"

class Http {
    private:
        int range[2];
        std::string reqst_file;
        std::string server_name;
        std::string httpver;
        std::string accpt;
        std::string accpt_charset;
        std::string accpt_encoding;
        std::string accpt_lang;
        std::string accpt_datetime;
        std::string authorization;
        std::string cache_control;
        std::string connection;
        std::string cookie;
        std::string content_len;
        std::string content_md5;
        std::string content_type;
        std::string date;
        std::string expect;
        std::string from;
        std::string host;
        std::string if_match;
        std::string if_modifd_since;
        std::string if_none_match;
        std::string if_range;
        std::string if_unmodif_since;
        std::string max_forwards;
        std::string origin;
        std::string pragma;
        std::string proxy_authztn;
        std::string referer;
        std::string transf_encod;
        std::string user_agent;
        std::string upgrade;
        std::string via;
        std::string warning;
        
        // non-standard
        bool dnt;
        std::string front_end_https;
        std::string proxy_connection;
        std::string x_att_deviceid;
        std::string x_csrf_token;
        std::string x_forwarded_for;
        std::string x_forwarded_host;
        std::string x_forwarded_proto;
        std::string x_http_method_override;
        std::string x_reqstd_with;
        std::string x_uidh;
        std::string x_wap_profile;

        std::string getfield(std::string, std::string, char);
        
        short reqsttype;
        int reply_status;
        int current_type;
        
        std::string header;
        t_byte * buffer;
        int buffer_size;
        
    public:
        enum Method {GET, POST, PUT, PATCH, DELETE, COPY, HEAD, OPTIONS, LINK, UNLINK, PURGE, RESPONSE};
        enum ContentType {JPG, PNG, GIF, ICO, MP4, OGG, WEBM, AUDIO_MP4, AUDIO_OGG, WAV, MP3, JS, JSON, TEXT, HTML, OCTET_STREAM, DASH};
        enum Status {OK = 200, CREATED = 201, ACCEPTED = 202, PARTIAL_CONTENT = 206, NOT_MODIFIED = 304, BAD_REQUEST = 400, NOT_FOUND = 404, NOT_ACCEPTED = 406, NOT_IMPLEMENTED = 501};
        enum BufferSize {MAX = 1024000};
        
        Http();
        virtual ~Http();
        
        int get_start_range_pos();
        int get_end_range_pos();
        std::string get_connection_state();
        std::string get_host();
        std::string get_reqsted_file();
        std::string get_referer();
        std::string get_user_agent();
        std::string get_accepted_types();
        std::string get_accepted_encoding();
        int get_reply_status();
        int get_content_type();
        std::string get_str_content_type();
        
        void setServerName(std::string);
        t_size getBinarySize();
        t_byte * getBinaryPacket();
        std::string getHeader();
        
        void processRequest(t_byte*);
        void processResponse(t_byte*);
        
        std::string createResponseHeader(t_size, std::string, int);
        std::string createRequestHeader(std::string, t_size, std::string, std::string, short);
        
        t_byte* createBinaryPacket(t_byte*, t_size);
        
        void clear();
        
        /**
         * Convert the file extension type to the HTTP Content-Type field format.
         *
         *
         */
        static std::string filename_to_content_type(std::string filename, int * ntype) {
            std::string type;
            std::string filetype = filename.substr(filename.rfind(".", filename.length() - 1) + 1, filename.length() - 1);
        
            if(!filetype.compare("m4s")) {
                *ntype = Http::ContentType::DASH;
                type = "application/octet-stream";
            } else if(!filetype.compare("mpd")) {
                *ntype = Http::ContentType::DASH;
                type = "application/octet-stream";
            } else if(!filetype.compare("mp4")) {
                *ntype = Http::ContentType::DASH;
                type = "video/mp4";
            } else if(!filetype.compare("webm")) {
                *ntype = Http::ContentType::WEBM;
                type = "video/webm";
            } else if(!filetype.compare("ogg")) {
                *ntype = Http::ContentType::OGG;
                type = "video/ogg";
            } else if(!filetype.compare("json")) {
                *ntype = Http::ContentType::JSON;
                type = "application/json; charset=UTF-8";
            } else if(!filetype.compare("js")) {
                *ntype = Http::ContentType::JS;
                type = "application/javascript; charset=UTF-8";
            } else if(!filetype.compare("html")) {
                *ntype = Http::ContentType::HTML;
                type = "text/html; charset=UTF-8";
            } else if(!filetype.compare("jpg")) {
                *ntype = Http::ContentType::JPG;
                type = "image/jpg";
            } else if(!filetype.compare("png")) {
                *ntype = Http::ContentType::PNG;
                type = "image/png";
            } else if(!filetype.compare("gif")) {
                *ntype = Http::ContentType::GIF;
                type = "image/gif";
            } else if(!filetype.compare("ico")) {
                *ntype = Http::ContentType::ICO;
                type = "image/x-icon";
            } else if(!filetype.compare("mp3")) {
                *ntype = Http::ContentType::MP3;
                type = "audio/mpeg";
            } else if(!filetype.compare("wav")) {
                *ntype = Http::ContentType::WAV;
                type = "audio/wav";
            } else {
                *ntype = Http::ContentType::TEXT;
                type = "text/html; charset=UTF-8";
            }
            
            return type;
        }
        
        static int content_type_to_int(std::string type) {
            if(type.find("application/json") != std::string::npos)
                return Http::ContentType::JSON;
            else if(type.find("application/octet-stream") != std::string::npos)
                return Http::ContentType::OCTET_STREAM;
            else if(type.find("application/javascript") != std::string::npos)
                return Http::ContentType::JS;
            else if(type.find("text/html") != std::string::npos)
                return Http::ContentType::TEXT;
            else if(type.find("video/mp4") != std::string::npos)
                return Http::ContentType::MP4;
            else if(type.find("video/webm") != std::string::npos)
                return Http::ContentType::WEBM;
            else if(type.find("video/ogg") != std::string::npos)
                return Http::ContentType::OGG;
            else if(type.find("image/jpg") != std::string::npos)
                return Http::ContentType::JPG;
            else if(type.find("image/png") != std::string::npos)
                return Http::ContentType::PNG;
            else if(type.find("image/gif") != std::string::npos)
                return Http::ContentType::GIF;
            else if(type.find("image/x-icon") != std::string::npos)
                return Http::ContentType::ICO;
            else if(type.find("audio/mp4") != std::string::npos)
                return Http::ContentType::AUDIO_MP4;
            else if(type.find("audio/ogg") != std::string::npos)
                return Http::ContentType::AUDIO_OGG;
            else if(type.find("audio/mpeg") != std::string::npos)
                return Http::ContentType::MP3;
            else if(type.find("audio/wav") != std::string::npos)
                return Http::ContentType::WAV;
            
            return -1;
        }
        
        static std::string int_to_content_type(int type) {
            if(type == Http::ContentType::JPG)
                return "image/jpg";
            else if(type == Http::ContentType::PNG)
                return "image/png";
            else if(type == Http::ContentType::GIF)
                return "image/gif";
            else if(type == Http::ContentType::ICO)
                return "image/x-icon";
            else if(type == Http::ContentType::MP4)
                return "video/mp4";
            else if(type == Http::ContentType::OGG)
                return "video/ogg";
            else if(type == Http::ContentType::WEBM)
                return "video/webm";
            else if(type == Http::ContentType::AUDIO_MP4)
                return "audio/mp4";
            else if(type == Http::ContentType::AUDIO_OGG)
                return "audio/ogg";
            else if(type == Http::ContentType::WAV)
                return "audio/wav";
            else if(type == Http::ContentType::MP3)
                return "audio/mpeg";
            else if(type == Http::ContentType::JS)
                return "application/javascript; charset=UTF-8";
            else if(type == Http::ContentType::TEXT || type == Http::ContentType::HTML)
                return "text/html; charset=UTF-8";
            else if(type == Http::ContentType::JSON)
                return "application/json; charset=UTF-8";
            else if(type == Http::ContentType::OCTET_STREAM)
                return "application/octet-stream";
        
            return "";
        }
};

#endif
