#include "http.h"

Http::Http() {
    this->clear();
    this->server_name       = "Lavid";
}

Http::~Http() {
    // NOT IMPLEMENTED
}

int Http::get_range_initial_pos() {
    return this->range[0];
}

int Http::get_range_final_pos() {
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
    //PRINT(std::endl << msg << std::endl << std::endl);
    
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
    
    //this-> = this->getfield(msg, , '\n');
    
    aux = this->getfield(msg, "Range: bytes=", '-');
    if(aux.compare("") != 0)
        this->range[0] = std::stoi(aux, &sz);
    //aux = this->getfield(msg, "Range: bytes=" + aux + '-', '\n');
    //this->range[1] = aux.compare("") != 0 ? std::stoi(aux, &sz) : 0;
}

std::string Http::generate(t_size filelen, std::string content_type, std::string last_modif_date) {
    t_size content_lengh = filelen - this->range[0];
    std::string resp;
    std::string msg    = "";
    std::string etag   = "\"\"";
    std::string time   = this->getDate();
    
    if(content_type.find("video/") != std::string::npos) {
        resp = RPLY_NOT_MODIFIED;
        msg = msg + "Accept-Ranges: bytes"             + "\r\n";
        msg = msg + "Cache-Control: public, max-age=0" + "\r\n";
        msg = msg + "Connection: keep-alive"           + "\r\n";
        
        if(this->if_modifd_since.compare(last_modif_date) != 0 || this->if_none_match.compare(etag) != 0) {
            resp = RPLY_PARTIAL_CONTENT;
            msg = msg + "Content-Length: "      + std::to_string(content_lengh)  + "\r\n";
            msg = msg + "Content-Range: bytes " + std::to_string(this->range[0]) + "-" + std::to_string(filelen - 1) + "/" + std::to_string(filelen) + "\r\n";
            msg = msg + "Content-Type: "        + content_type + "\r\n";
        }
        
        msg = msg + "Date: "          + time            + "\r\n";
        msg = msg + "Etag: "          + etag            + "\r\n";
        msg = msg + "Last-Modified: " + last_modif_date + "\r\n";
        msg = msg + "X-Powered-By: "  + server_name     + "\r\n\r\n";
    } else {
        resp = RPLY_NOT_FOUND;
        msg = msg + "Connection: keep-alive"                 + "\r\n";
        msg = msg + "Content-Length: " + std::to_string(70)  + "\r\n";
        msg = msg + "Content-Type: text/html; charset=utf-8" + "\r\n";
        msg = msg + "Date: "           + time                + "\r\n";
        msg = msg + "X-Powered-By: "   + server_name         + "\r\n";
        msg = msg + "x-content-type-options: nosniff"        + "\r\n\r\n";
    }
    
    return "HTTP/1.1 " + resp + "\r\n" + msg;
}

std::string Http::getfield(std::string src, std::string mark, char sep) {
    std::string value = "";
    
    for(int pos = src.find(mark) + mark.length(); pos != std::string::npos && src.at(pos) != '\0' && src.at(pos) != sep; pos++)
        value += src.at(pos);

    return value;    
}

void Http::clear() {
    this->range[0]          = 0;
    this->range[1]          = 0;
    this->reqst_file        = "";
    this->httpver      = "";
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

/*    
GET /video.mp4 HTTP/1.1
Host: 192.168.77.132:3000
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; rv:43.0) Gecko/20100101 Firefox/43.0
Accept: video/webm,video/ogg,video/*;q=0.9,application/ogg;q=0.7,audio/*;q=0.6,**;q=0.5
Accept-Language: pt-BR,pt;q=0.8,en-US;q=0.5,en;q=0.3
DNT: 1
Range: bytes=0-
Referer: http://192.168.77.132:3000/
Connection: keep-alive    
If-Modified-Since: Mon, 21 Dec 2015 01:38:55 GMT
If-None-Match: W/"11c8-151c2307798"    
*/    















