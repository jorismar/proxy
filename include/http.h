#ifndef HTTP_H
#define HTTP_H

// Informational Responses
#define RPLY_CONTINUE               "100"
#define RPLY_SWITCHING              "101"

// Sucessful Responses
#define RPLY_OK                     "200 OK"
#define RPLY_CREATED                "201"
#define RPLY_ACCEPTED               "202"
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
#define RPLY_BAD_REQUEST            "400"
#define RPLY_UNAUTHORIZED           "401"
#define RPLY_PAYMENT_REQUIRED       "402"
#define RPLY_FORBIDDEN              "403"
#define RPLY_NOT_FOUND              "404 Not Found"
#define RPLY_METHOD_NOT_ALLWED      "405"
#define RPLY_NOT_ACCEPTABLE         "406"
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
#define RPLY_NOT_IMPLEMENTED        "501"
#define RPLY_BAD_GATEWAY            "502"
#define RPLY_SERVIC_UNAVAILABLE     "503"
#define RPLY_GATEWAY_TIMEOUT        "504"
#define RPLY_HTTP_VER_NOT_SUPPT     "505"
#define RPLY_VARNT_ALSO_NEGOT       "506"
#define RPLY_VARNT_ALSO_NEGOTIATION "507"
#define RPLY_NETWRK_AUTHENT_REQ     "511"

//Content Type
#define TYPE_TEXT   0   // html
#define TYPE_IMAGE  1   // jpg, png, gif, etc.
#define TYPE_MEDIA  2   // mp3, mp4, ogg, webm, m4s, etc.
#define TYPE_VIDEO  3   // mp4, ogg, webm
#define TYPE_AUDIO  4   // mp4, mp3
//#define TYPE_APP    5
#define TYPE_ANY    6   // */*

// Connection
#define CLOSE       "close"
#define KEEP_ALIVE  "keep-alive"

#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "types.h"
#include "datapacket.h"

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
        
    public:
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
        
        void setServerName(std::string);
        
        void process(DataPacket*);
        std::string generate(t_size, std::string, std::string);
        void clear();
        
        static std::string getDate() {
            time_t rawtime;
            struct tm * timeinfo;
            char buffer [80];
            
            time (&rawtime);
            timeinfo = localtime (&rawtime);
            
            strftime (buffer, 80,"%a, %d %b %Y %T %Z", timeinfo);
            
            return std::string(buffer);
        }
};

#endif
