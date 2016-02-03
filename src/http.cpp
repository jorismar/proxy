#include "http.h"

Http::Http() {
    //this->clear();
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

int Http::get_reply_status() {
    return reply_status;
}

int Http::get_content_type() {
    if(this->content_type.find("application/json")  != std::string::npos)
        return Http::ContentType::JSON;
    else return -1;
}

void Http::setServerName(std::string name) {
    this->server_name = name;
}

std::string Http::getfield(std::string src, std::string mark, char sep) {
    int src_size = src.length(), mark_start_pos = src.find(mark);
    std::string value = "";
    
    if(mark_start_pos != std::string::npos) {
        for(int pos = mark_start_pos + mark.length(); pos < src_size && src.at(pos) != sep; pos++) {
            value += src.at(pos);
        }
    }
    
    return value;    
}

void Http::processResponse(t_byte * header) {
    std::string msg(header);
    
    try {
        this->reply_status = std::stoi(this->getfield(msg, " ", ' '), NULL);
    } catch(...) {
        PRINT("[ERROR] Could not read the reply status code")
    }
}
    
void Http::processRequest(t_byte * header) {
    std::string msg(header);
    
    if(msg.find("GET") != std::string::npos) {
        this->reqst_file    = this->getfield(msg, "GET ", ' ');
        this->reqsttype     = Http::Method::GET;
    } else if(msg.find("POST") != std::string::npos) {
        this->reqst_file    = this->getfield(msg, "POST ", ' ');
        this->reqsttype     = Http::Method::POST;
        this->content_type  = this->getfield(msg, "Content-Type: ", '\r');
    }
    
    this->range[0] = -1;
    this->range[1] = -1;

    {
        std::string aux;
        std::string::size_type sz;

        aux = this->getfield(msg, "Range: bytes=", '-');
        
        if(aux.length() > 0) {
            //try {
                try {
                    this->range[0] = std::stoi(aux, &sz);
                } catch(...) {}
                //} catch(const std::invalid_argument& ex) {}
            //} catch (const std::out_of_range& ex) {}
            
            aux = this->getfield(msg, "Range: bytes=" + aux + '-', '\n');
            
            try {
                this->range[1] = std::stoi(aux, &sz);
            } catch(...) {}
        }
    }
}

std::string Http::genResponse(t_size filelen, std::string filetype, std::string last_modif_date, int status) {
    std::string length  = std::to_string(filelen > 0 ? filelen : 0);
    std::string msg     = "";
    std::string resp;
    std::string etag    = "\"\"";
    std::string type    = "";
    std::string aux     = "";
    std::string connection;
    
    if(status < Http::Status::NOT_IMPLEMENTED) {
        if(status == Http::Status::NOT_MODIFIED) {
            resp = RPLY_NOT_MODIFIED;
            connection = "keep-alive";
        } else {
            if(status >= 200 && status < 300) {
                if(status == Http::Status::OK) {
                    resp = RPLY_OK;
                    
                } else if(status == Http::Status::PARTIAL_CONTENT) {
                    resp = RPLY_PARTIAL_CONTENT;
                } else if(status == Http::Status::ACCEPTED) {
                    resp = RPLY_ACCEPTED;
                } else if(status == Http::Status::CREATED) {
                    resp = RPLY_CREATED;
                }
                
                if(this->range[0] >= 0)
                    resp = RPLY_PARTIAL_CONTENT;
                else this->range[0] = 0;

                msg += "Content-Range: bytes " + std::to_string(this->range[0]) + "-" + std::to_string(filelen) + "/" + length + "\r\n";
                msg += "Accept-Ranges: bytes\r\n";
                msg += "Cache-Control: public, max-age=0\r\n";
                connection = "keep-alive";

                type = filetype;
            } else if(status == Http::Status::NOT_FOUND) {
                resp = RPLY_NOT_FOUND;
                aux = "<center><br><br><font size=\"8\">404</font><br><font size=\"6\">NOT FOUND</font></center>";
                msg += "x-content-type-options: nosniff\r\n";
                type = "text/html; charset=UTF-8";
                length = std::to_string(aux.length());
                connection = "close";
            } else if(status == Http::Status::NOT_ACCEPTED || status == Http::Status::BAD_REQUEST) {
                resp = status == Http::Status::NOT_ACCEPTED ? RPLY_NOT_ACCEPTABLE : RPLY_BAD_REQUEST;
                msg += "Cache-Control: no-cache, no-store, max-age=0\r\n";
                connection = "close";
                type = "text/html; charset=UTF-8";
            }
            
            msg += "Content-Type: "   + type            + "\r\n";
            msg += "Content-Length: " + length          + "\r\n";
        }
    
        msg += "Connection: " + connection + "\r\n";
    } else {
        resp = RPLY_NOT_IMPLEMENTED;
    }
    
    msg += "Access-Control-Allow-Origin: *\r\n";
    msg += "Access-Control-Allow-Headers: X-Requested-With, Content-Type, X-Codingpedia, X-HTTP-Method-Override\r\n";
    msg += "Date: "           + this->getDate()     + "\r\n";
    msg += "X-Powered-By: "   + this->server_name   + "\r\n";
    
    return "HTTP/1.1 " + resp + "\r\n" + msg + "\r\n" + aux;
}

std::string Http::genRequest(std::string filename, t_size filesize, short content_type, std::string accept_type, std::string host, short headtype) {
    std::string msg;
    std::string content = content_type == Http::ContentType::JSON ? "application/json" : "text/html; charset=UTF-8";
    
    if(headtype == Http::Method::GET) msg = "GET /" + filename + " HTTP/1.1\r\n";
        else if(headtype == Http::Method::POST) msg = "POST /" + filename + " HTTP/1.1\r\n";

    msg += "Host: " + host + "\r\n";
    msg += "Connection: keep-alive\r\n";
    msg += "Content-Type: " + content + "\r\n";
    msg += "Content-Length: " + std::to_string(filesize) + "\r\n";
    msg += "Cache-Control: no-cache\r\n";
    msg += "Accept: " + accept_type + "\r\n";

    return msg + "\r\n";
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
    
    this->reqsttype = 0;
    this->reply_status = 0;
}
