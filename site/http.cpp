#include "http.h"

Http::Http() {
    this->clear();
    this->server_name       = "Lavid/Jorismar";
}

Http::~Http() {
    // NOT IMPLEMENTED
}

int Http::get_start_range_pos() {
    return this->range[0];
}

int Http::get_end_range_pos() {
    return this->range[1];
}

std::string Http::get_connection_state() {
    return this->connection;
}

std::string Http::get_reqsted_file() {
    return this->reqst_file;
}

std::string Http::get_referer() {
    return this->referer;
}

std::string Http::get_user_agent() {
    return this->user_agent;
}

std::string Http::get_accepted_types() {
    return this->accpt;
}

std::string Http::get_accepted_encoding() {
    return this->accpt_encoding;
}

void Http::setServerName(std::string name) {
    this->server_name = name;
}

void Http::process(DataPacket * header) {
    std::string aux;
    std::string::size_type sz;
    std::string msg(header->get());
    
    this->reqst_file = this->getfield(msg, "GET ", ' ');
    this->httpver    = this->getfield(msg, "HTTP/", '\n');
    this->host       = this->getfield(msg, "Host: ", '\n');
    this->user_agent = this->getfield(msg, "User-Agent: ", '\n');
    this->accpt      = this->getfield(msg, "Accept: ", '\n');
    this->accpt_lang = this->getfield(msg, "Accept-Language: ", '\n');
    this->referer    = this->getfield(msg, "Referer: ", '\n');
    this->connection = this->getfield(msg, "Connection: ", '\n');
    this->if_modifd_since = this->getfield(msg, "If-Modified-Since: ", '\n');
    this->if_none_match = this->getfield(msg, "If-None-Match: ", '\n');
    
    aux = this->getfield(msg, "Range: bytes=", '-');
    
    try {
        try {
            this->range[0] = std::stoi(aux, &sz);
        } catch(const std::invalid_argument& ex) {
            this->range[0] = 0;
        }
    } catch (const std::out_of_range& ex) {
        this->range[0] = 0;
    }
    
    aux = this->getfield(msg, "Range: bytes=" + aux + '-', '\n');
    
    try {
        try {
            this->range[1] = std::stoi(aux, &sz);
        } catch(const std::invalid_argument& ex) {
            this->range[1] = 0;
        }
    } catch (const std::out_of_range& ex) {
        this->range[1] = 0;
    }
    
    this->reqsttype = this->accpt.find("text/") != std::string::npos ? TYPE_TEXT :
                        this->accpt.find("image/") != std::string::npos ? TYPE_IMAGE :
                            this->accpt.find("video/") != std::string::npos && this->accpt.find("audio/") != std::string::npos ? TYPE_MEDIA :
                                //this->accpt.find("application/") != std::string::npos ? TYPE_APP :
                                    this->accpt.find("video/") != std::string::npos ? TYPE_VIDEO :
                                        this->accpt.find("audio/") != std::string::npos ? TYPE_AUDIO :
                                            this->accpt.find("*/*") != std::string::npos ? TYPE_ANY : -1;
}

std::string Http::generate(t_size filelen, std::string filetype, std::string last_modif_date) {
    std::string length = std::to_string(filelen > 0 ? filelen : 70);
    std::string msg    = "";
    std::string resp;
    std::string etag   = "\"\"";
    std::string time   = this->getDate();
    std::string type   = "";
    
    if(filelen == 0) {
        resp = RPLY_NOT_FOUND;
        msg = msg + "x-content-type-options: nosniff\r\n";
        msg = msg + "Content-Type: text/html; charset=UTF-8\r\n";
        msg = msg + "Content-Length: " + length  + "\r\n";
    } else {
        msg = msg + "Accept-Ranges: bytes"             + "\r\n";
        msg = msg + "Cache-Control: public, max-age=0" + "\r\n";

        if(this->if_modifd_since.compare(last_modif_date) != 0 || this->if_none_match.compare(etag) != 0) {
            resp = RPLY_OK;
    
            if(this->reqsttype == TYPE_TEXT) {
                type = type + "text/" + (!filetype.compare("htm") ? "html" : filetype) + "; charset=UTF-8";
            } else if(this->reqsttype == TYPE_IMAGE) {
                type = type + "image/" + filetype;
            } else {
                resp = RPLY_PARTIAL_CONTENT;
                msg = msg + "Content-Range: bytes " + std::to_string(this->range[0]) + "-" + std::to_string(filelen - 1) + "/" + length + "\r\n";
                
                if(this->reqsttype == TYPE_MEDIA)
                    type = !filetype.compare("mp4") || !filetype.compare("webm") || !filetype.compare("ogg") || !filetype.compare("m4s") ? type + "video/" + filetype : type + "audio/" + filetype;
                else if(this->reqsttype == TYPE_VIDEO)
                    type = type + "video/" + filetype;
                else if(this->reqsttype == TYPE_AUDIO)
                    type = type + "audio/" + filetype;
                //else if(this->reqsttype == TYPE_APP)
                    //type = type + "application/" + "javascript";
                else if(this->reqsttype == TYPE_ANY) {
                    type =  !filetype.compare("html") ? type + "text/" + filetype + "; charset=UTF-8" :
                                !filetype.compare("jpg") || !filetype.compare("png") || !filetype.compare("gif") ? type + "image/" + filetype :
                                    !filetype.compare("mp4") || !filetype.compare("webm") || !filetype.compare("ogg") || !filetype.compare("m4s") ? type + "video/" + filetype :
                                        !filetype.compare("mp3") || !filetype.compare("acc") ? type + "audio/" + filetype :
                                            !filetype.compare("js") ? "application/javascript" :
                                                !filetype.compare("ico") ? "image/x-icon" : "";
                                                //!filetype.compare("ico") ? "image/vnd.microsoft.icon" :
                                                    
                    if(type.find("video/") == std::string::npos && type.find("audio/") == std::string::npos)
                        resp = RPLY_OK;
                }
            }
                
            msg = msg + "Content-Type: "   + type            + "\r\n";
            msg = msg + "Content-Length: " + length          + "\r\n";
        } else resp = RPLY_NOT_MODIFIED;

        msg = msg + "Etag: "           + etag            + "\r\n";
        msg = msg + "Last-Modified: "  + last_modif_date + "\r\n";
    }
    
    msg = msg + "Connection: keep-alive"                 + "\r\n";
    msg = msg + "Date: "           + time                + "\r\n";
    msg = msg + "X-Powered-By: "   + this->server_name   + "\r\n\r\n";
    
    return "HTTP/1.1 " + resp + "\r\n" + msg;
}

std::string Http::getfield(std::string src, std::string mark, char sep) {
    std::string value = "";
    
    for(int pos = src.find(mark) + mark.length(); pos < src.length() && pos != std::string::npos && src.at(pos) != '\0' && src.at(pos) != sep; pos++)
        value += src.at(pos);

    return value;    
}

void Http::clear() {
    this->range[0]          = 0;
    this->range[1]          = 0;
    this->reqst_file        = "";
    this->httpver           = "";
    this->accpt             = "";
    this->accpt_charset     = "";
    this->accpt_encoding    = "";
    this->accpt_lang        = "";
    this->accpt_datetime    = "";
    this->authorization     = "";
    this->cache_control     = "";
    this->connection        = CLOSE;
    this->cookie            = "";
    this->content_len       = "";
    this->content_md5       = "";
    this->content_type      = "";
    this->date              = "";
    this->expect            = "";
    this->from              = "";
    this->host              = "";
    this->if_match          = "";
    this->if_modifd_since   = "";
    this->if_none_match     = "";
    this->if_range          = "";
    this->if_unmodif_since  = "";
    this->max_forwards      = "";
    this->origin            = "";
    this->pragma            = "";
    this->proxy_authztn     = "";
    this->referer           = "";
    this->transf_encod      = "";
    this->user_agent        = "";
    this->upgrade           = "";
    this->via               = "";
    this->warning           = "";
    
    // non-standard
    this->dnt               = 0;
    this->front_end_https   = "";
    this->proxy_connection  = "";
    this->x_att_deviceid    = "";
    this->x_csrf_token      = "";
    this->x_forwarded_for   = "";
    this->x_forwarded_host  = "";
    this->x_forwarded_proto = "";
    this->x_http_method_override = "";
    this->x_reqstd_with     = "";
    this->x_uidh            = "";
    this->x_wap_profile     = "";
}
