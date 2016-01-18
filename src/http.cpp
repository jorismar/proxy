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

void Http::processRequest(t_byte * header) {
    std::string msg(header);
    
    if(msg.find("GET") != std::string::npos) {
        this->is_get_reqst = true;
        this->reqst_file = this->getfield(msg, "GET ", ' ');
        this->reqsttype = Http::Type::GET;
    } else if(msg.find("POST") != std::string::npos) {
        this->reqst_file = this->getfield(msg, "POST ", ' ');
        this->reqsttype = Http::Type::POST;
    }
//  this->httpver    = this->getfield(msg, "HTTP/", '\n');
//  this->host       = this->getfield(msg, "Host: ", '\n');
//  this->user_agent = this->getfield(msg, "User-Agent: ", '\n');
//  this->accpt      = this->getfield(msg, "Accept: ", '\n');
//  this->accpt_lang = this->getfield(msg, "Accept-Language: ", '\n');
//  this->referer    = this->getfield(msg, "Referer: ", '\n');
//  this->connection = this->getfield(msg, "Connection: ", '\n');
//  this->if_modifd_since = this->getfield(msg, "If-Modified-Since: ", '\n');
//  this->if_none_match = this->getfield(msg, "If-None-Match: ", '\n');
    
    this->range[0] = -1;
    this->range[1] = -1;

    {
        std::string aux;
        std::string::size_type sz;

        aux = this->getfield(msg, "Range: bytes=", '-');
        
        if(aux.length() > 0) {
            try {
                try {
                    this->range[0] = std::stoi(aux, &sz);
                } catch(const std::invalid_argument& ex) {}
            } catch (const std::out_of_range& ex) {}
            
            aux = this->getfield(msg, "Range: bytes=" + aux + '-', '\n');
            
            try {
                try {
                    this->range[1] = std::stoi(aux, &sz);
                } catch(const std::invalid_argument& ex) {}
            } catch (const std::out_of_range& ex) {}
        }
    }
}

std::string Http::genResponse(t_size filelen, std::string filetype, std::string last_modif_date) {
    std::string length = std::to_string(filelen > 0 ? filelen : 0);
    std::string msg    = "";
    std::string resp;
    std::string etag   = "\"\"";
    std::string time   = this->getDate();
    std::string aux    = "";
    
    if(filelen == 0) {
        resp = RPLY_NOT_FOUND;
        aux = "<center><br><br><font size=\"8\">404</font><br><font size=\"6\">PAGE NOT FOUND</font></center>";
        msg = msg + "x-content-type-options: nosniff\r\n";
        msg = msg + "Content-Type: text/html; charset=UTF-8\r\n";
        msg = msg + "Content-Length: " + std::to_string(aux.length())  + "\r\n";
    } else {
        msg = msg + "Accept-Ranges: bytes"             + "\r\n";
        msg = msg + "Cache-Control: public, max-age=0" + "\r\n";

        if(this->if_modifd_since.compare(last_modif_date) != 0 || this->if_none_match.compare(etag) != 0) {
            resp = RPLY_OK;
    
            if(this->range[0] >= 0)
                resp = RPLY_PARTIAL_CONTENT;
            else this->range[0] = 0;
            
            msg = msg + "Content-Range: bytes " + std::to_string(this->range[0]) + "-" + std::to_string(filelen - 1) + "/" + length + "\r\n";

            std::string type = "";
            
            type =  !filetype.compare("html") || !filetype.compare("htm") ? type + "text/html; charset=UTF-8" :
                        !filetype.compare("jpg") || !filetype.compare("png") || !filetype.compare("gif") ? type + "image/" + filetype :
                            !filetype.compare("mp4") || !filetype.compare("webm") || !filetype.compare("ogg") ? type + "video/" + filetype :
                                !filetype.compare("mp3") || !filetype.compare("acc") ? type + "audio/" + filetype :
                                    !filetype.compare("js") ? "application/javascript" :
                                        !filetype.compare("mpd") || !filetype.compare("m4s") ? "application/octet-stream" :
                                            !filetype.compare("json") ? "application/json" :
                                                !filetype.compare("ico") ? "image/x-icon" : "";
                                            //!filetype.compare("ico") ? "image/vnd.microsoft.icon" :
        
            msg = msg + "Content-Type: "   + type            + "\r\n";
            msg = msg + "Content-Length: " + length          + "\r\n";
        } else resp = RPLY_NOT_MODIFIED;

        msg = msg + "Etag: "           + etag            + "\r\n";
        msg = msg + "Last-Modified: "  + last_modif_date + "\r\n";
    }
    
    msg = msg + "Connection: keep-alive"                 + "\r\n";
    msg = msg + "Date: "           + time                + "\r\n";
    msg = msg + "X-Powered-By: "   + this->server_name   + "\r\n\r\n";
    
    return "HTTP/1.1 " + resp + "\r\n" + msg + aux;
}

std::string Http::genRequest(std::string filename, t_size filesize, std::string accept_type, std::string host, short headtype) {
    std::string msg;
    
    if(headtype == Http::Type::GET) {
        // NOT IMPLEMENTED YET
    } else if(headtype == Http::Type::POST) {
        msg = "POST /" + filename + " HTTP/1.1\r\n";
        msg = msg + "Host: " + host + "\r\n";
        msg = msg + "Connection: keep-alive\r\n";
        msg = msg + "Content-Length: " + std::to_string(filesize) + "\r\n";
    //  msg = msg + "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36\r\n";
        msg = msg + "Cache-Control: no-cache\r\n";
        msg = msg + "Accept: " + accept_type + "\r\n";
        msg = msg + "DNT: 1\r\n";
        msg = msg + "Accept-Encoding: gzip, deflate\r\n";
        msg = msg + "Accept-Language: pt-BR,pt;q=0.8,en-US;q=0.6,en;q=0.4\r\n";
    }

    return msg + "\r\n";
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
    this->is_get_reqst      = false;
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
