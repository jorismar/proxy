/**
 * \file 	http.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

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

#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "util.h"

/**
 * \brief   This class implements the essentials operations for HTTP protocol.
 * 
 * \headerfile http.h
 */

class Http {
    private:
    /**************************************************************************************/
        int range[2];               // Requested content range. [0] begin, [1] end
        std::string reqst_file;     // Requested file path.
        std::string server_name;    // Server name
        std::string content_type;   // Requested content type.

        short reqsttype;            // HTTP request type.
        int reply_status;           // Received reply status.
        int current_type;           // Requested content type (int format).
        
        std::string header;         // Container generated header.
        t_byte * buffer;            // Buffer for binary packet generated.
        int buffer_size;            // Buffer size

    /**************************************************************************************/

        //std::string httpver;
        //std::string accpt;
        //std::string accpt_charset;
        //std::string accpt_encoding;
        //std::string accpt_lang;
        //std::string accpt_datetime;
        //std::string authorization;
        //std::string cache_control;
        //std::string cookie;
        //std::string connection;
        //std::string content_len;
        //std::string content_md5;
        //std::string date;
        //std::string expect;
        //std::string from;
        //std::string host;
        //std::string if_match;
        //std::string if_modifd_since;
        //std::string if_none_match;
        //std::string if_range;
        //std::string if_unmodif_since;
        //std::string max_forwards;
        //std::string origin;
        //std::string pragma;
        //std::string proxy_authztn;
        //std::string referer;
        //std::string transf_encod;
        //std::string user_agent;
        //std::string upgrade;
        //std::string via;
        //std::string warning;
        
        /* non-standard */
        //bool dnt;
        //std::string front_end_https;
        //std::string proxy_connection;
        //std::string x_att_deviceid;
        //std::string x_csrf_token;
        //std::string x_forwarded_for;
        //std::string x_forwarded_host;
        //std::string x_forwarded_proto;
        //std::string x_http_method_override;
        //std::string x_reqstd_with;
        //std::string x_uidh;
        //std::string x_wap_profile;


        /**
         * \brief   Takes a string, searching for a particular brand and collecting a
         *          substring starting after the last marker character found even to the
         *          termination character.
         *
         * \param   src     Origin source.
         * \param   mark    Mark to be find.
         * \param   end     End character
         *
         * \return  Return a string containing the substring found.
         */
        std::string getfield(std::string src, std::string mark, char end);

    public:

        /**
         * \enum    Method
         * 
         * \brief   Constants of the supported methods requests.
         */
        enum Method {
            GET,        /*!< Get */
            POST,       /*!< Post */
            PUT,        /*!< Put */
            PATCH,      /*!< Patch */
            DELETE,     /*!< Delete */
            COPY,       /*!< Copy */
            HEAD,       /*!< Head */
            OPTIONS,    /*!< Options */
            LINK,       /*!< Link */
            UNLINK,     /*!< Unlink */
            PURGE,      /*!< Purge */
            RESPONSE    /*!< Response */
        };

        /**
         * \enum    ContentType
         * 
         * \brief   Constants of the supported content types.
         */
        enum ContentType {
            JPG,            /*!< Image jpeg  format */
            PNG,            /*!< Image png format */
            GIF,            /*!< Image gif format */
            ICO,            /*!< Image ico format */
            MP4,            /*!< Video mp4 (h264 codec) format */
            OGG,            /*!< Video ogg (theora codec) format */
            WEBM,           /*!< Video webm (vp8 codec) format */
            AUDIO_MP4,      /*!< Audio mp4 (aac/mp3 codec) format */
            AUDIO_OGG,      /*!< Audio ogg (vorbis/opus codec) format */
            WAV,            /*!< Audio wav (pcm codec) format */
            MP3,            /*!< Audio mp3 (mp3 codec) format */
            JS,             /*!< Text javascript format */
            JSON,           /*!< Text JSON script format */
            TEXT,           /*!< Text format */
            HTML,           /*!< Text HTML code format */
            OCTET_STREAM,   /*!< Byte stream format */
            DASH            /*!< Dash files type (mpd, mp4 and m4s) */
        };

        /**
         * \enum    Status
         * 
         * \brief   Constants of the supported responses status.
         */
        enum Status {
            OK = 200,               /*!< Status code 200 OK */
            CREATED = 201,          /*!< Status code 201 Created */
            ACCEPTED = 202,         /*!< Status code 202 Accepted */
            PARTIAL_CONTENT = 206,  /*!< Status code 206 Partial Content */
            NOT_MODIFIED = 304,     /*!< Status code 304 Not Modified */
            BAD_REQUEST = 400,      /*!< Status code 400 Bad Request */
            NOT_FOUND = 404,        /*!< Status code 404 Not Found */
            NOT_ACCEPTED = 406,     /*!< Status code 406 Not Acceptable */
            NOT_IMPLEMENTED = 501   /*!< Status code 501 Not Implemented */
        };

        /**
         * \enum    BufferSize
         * 
         * \brief   Constants of the buffer sizes.
         */
        enum BufferSize {
            DEFAULT = 1024,     /*!< Default value of HTTP buffer size 1 KB */
            MAX = 1024000       /*!< Max value of HTTP buffer size 1 MB */
        };
        
        /**
         * \brief   Constructor
         */
        Http();

        /**
         * \brief   Destructor
         */
        virtual ~Http();
        

        /**
         * \brief   Get the start byte range position.
         *
         * \return  Return the start range byte position.
         */
        int getStartRangePos();

        /**
         * \brief   Get the file requested by the client.
         *
         * \return  Return the requested file.
         */
        std::string getReqstedFile();

        /**
         * \brief   Get the remote server reply status.
         *
         * \return  Return the status number code.
         */
        int getReplyStatus();

        /**
         * \brief   Get the requested content type.
         *
         * \return  Return the value of requested content type.
         */
        int getContentType();

        /**
         * \brief   Get the requested content type.
         *
         * \return  Return the string of requested content type.
         */
        std::string getStrContentType();
        

        /**
         * \brief   Set the server name.
         *
         * \param   name    New name of server.
         */
        void setServerName(std::string name);

        /**
         * \brief   Get the generated binary packet pointer.
         *
         * \return  Return the binary packet pointer.
         */
        t_byte * getBinaryPacket();

        /**
         * \brief   Get the generated binary packet size.
         *
         * \return  Return the binary packet size.
         */
        t_size getBinarySize();

        /**
         * \brief   Get the generated header.
         *
         * \return  Return a string of the generated header.
         */
        std::string getHeader();

        /**
         * \brief   Processes a request header and collects the data contained therein.
         *
         * \param   header  Byte pointer of binary packet from a HTTP/TCP connection.
         */
        void processRequest(t_byte* header);

        /**
         * \brief   Processes a response header and collects the data contained therein.
         *
         * \param   header  Byte pointer of binary packet from a HTTP/TCP connection.
         */
        void processResponse(t_byte* header);

        /**
         * \brief   Create a response header.
         *
         * \param   filelen         Size of file to be send with the header.
         * \param   content_type    Content type of the file.
         * \param   status          HTTP status code value.
         *
         * \return  Return a string containing the generated header.
         */
        std::string createResponseHeader(t_size filelen, std::string content_type, int status);

        /**
         * \brief   Create a request header.
         * 
         * \param   filename        Name of the file to be send with the header.
         * \param   filesize        Size of file.
         * \param   content_type    Content type of the file.
         * \param   host            Host IP:Port address
         * \param   headtype        HTTP request method type.
         *
         * \return  Return a string containing the generated header.
         */
        std::string createRequestHeader(std::string filename, t_size filesize, std::string content_type, std::string host, short headtype);

        /**
         * \brief   Create a binary package with the generated header and the binary of the file to be sent.
         *
         * \param   file_bin    Pointer to the binary file buffer.
         * \param   file_size   Size of file buffer.
         *
         * \return  Return a pointer of the binary packet containig the generated header and the binary data of the file.
         */
        t_byte* createBinaryPacket(t_byte * file_bin, t_size file_size);

        /**
         * \brief   Convert the file extension type to the HTTP/Content-type format.
         *
         * \param   filename    Name of file.
         * \param   ntype       Pointer of a int variable to storage the Content-type value format.
         *
         * \return  Return a string containing the Content-type.
         */
        static std::string filename2ContentType(std::string filename, int * ntype) {
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

        /**
         * \brief   Convert the Content-type string format to the Content-type int value.
         *
         * \param   type    Content-type in string format.
         *
         * \return  Return the Content-type in the int value format.
         */
        static int contentType2int(std::string type) {
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

        /**
         * \brief   Convert the Content-type int value to the Content-type string format.
         *
         * \param   type    Content-type in int value format.
         *
         * \return  Return the Content-type in the string format.
         */
        static std::string int2ContentType(int type) {
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
