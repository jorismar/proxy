#include "http.h"

/******************************************************************************************/

Http::Http() {
    this->current_type      = -1;
    this->buffer            = NULL;
    this->server_name       = "Lavid/Jorismar";
}

/******************************************************************************************/

Http::~Http() {
    if(this->buffer != NULL)
        free(buffer);
}

/******************************************************************************************/

int Http::get_start_range_pos() {
    return this->range[0];
}

/******************************************************************************************/

int Http::get_end_range_pos() {
    return this->range[1];
}

/******************************************************************************************

std::string Http::get_connection_state() {
    return this->connection;
}

/******************************************************************************************/

std::string Http::get_reqsted_file() {
    return this->reqst_file;
}

/******************************************************************************************

std::string Http::get_referer() {
    return this->referer;
}

/******************************************************************************************

std::string Http::get_user_agent() {
    return this->user_agent;
}

/******************************************************************************************

std::string Http::get_accepted_types() {
    return this->accpt;
}

/******************************************************************************************

std::string Http::get_accepted_encoding() {
    return this->accpt_encoding;
}

/******************************************************************************************/

int Http::get_reply_status() {
    return reply_status;
}

/******************************************************************************************/

int Http::get_content_type() {
    if(this->current_type < 0)
        this->current_type = Http::content_type_to_int(this->content_type);
    
    return this->current_type;
}

/******************************************************************************************/

std::string Http::get_str_content_type() {
    return this->content_type;
}

/******************************************************************************************/

void Http::setServerName(std::string name) {
    this->server_name = name;
}

/******************************************************************************************/

t_size Http::getBinarySize() {
    return this->buffer_size;
}

/******************************************************************************************/

t_byte * Http::getBinaryPacket() {
    return this->buffer;
}

/******************************************************************************************/

std::string Http::getHeader() {
    return this->header;
}

/******************************************************************************************/

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

/******************************************************************************************/

void Http::processResponse(t_byte * header) {
    std::string msg(header);
    
    try {
        this->reply_status = std::stoi(this->getfield(msg, " ", ' '), NULL);
    } catch(...) {
        PRINT("[ERROR] Could not read the reply status code")
    }
}
    
/******************************************************************************************/

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

    std::string aux;
    std::string::size_type sz;

    aux = this->getfield(msg, "Range: bytes=", '-');
    
    if(aux.length() > 0) {
        try {
            this->range[0] = std::stoi(aux, &sz);
        } catch(...) {
            this->range[0] = -1;
        }

        aux = this->getfield(msg, "Range: bytes=" + aux + '-', '\n');

        try {
            this->range[1] = std::stoi(aux, &sz);
        } catch(...) {
            this->range[1] = -1;
        }
    }
}

/******************************************************************************************/

std::string Http::createResponseHeader(t_size filelen, std::string content_type, int status) {
    std::string resp, connection;
    std::string type    = "text/html; charset=UTF-8";
    std::string filesize = std::to_string(filelen);
    std::string length  = filesize;
    
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
                
                if(this->range[0] >= 0) {
                    resp = RPLY_PARTIAL_CONTENT;
                    length = std::to_string(filelen - this->range[0]);
                } else this->range[0] = 0;

                connection = "keep-alive";
                type = content_type;

                this->header = "Content-Range: bytes " + std::to_string(this->range[0]) + "-" + std::to_string(filelen - 1) + "/" + filesize + "\r\nAccept-Ranges: bytes\r\nCache-Control: public, max-age=0\r\n";
            } else if(status == Http::Status::NOT_FOUND) {
                resp = RPLY_NOT_FOUND;
//              aux = "<center><br><br><font size=\"8\">404</font><br><font size=\"6\">NOT FOUND</font></center>";
                this->header = "x-content-type-options: nosniff\r\n";
//              length = std::to_string(aux.length());
                connection = "close";
            } else if(status == Http::Status::NOT_ACCEPTED || status == Http::Status::BAD_REQUEST) {
                resp = status == Http::Status::NOT_ACCEPTED ? RPLY_NOT_ACCEPTABLE : RPLY_BAD_REQUEST;
                this->header = "Cache-Control: no-cache, no-store, max-age=0\r\n";
                connection = "close";
            }
            
            this->header += "Content-Type: " + type + "\r\nContent-Length: " + length + "\r\n";
        }
    
        this->header += "Connection: " + connection + "\r\n";
    } else {
        resp = RPLY_NOT_IMPLEMENTED;
    }
    
    this->header += "Access-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: X-Requested-With, Content-Type, X-Codingpedia, X-HTTP-Method-Override\r\nDate: " + getDate("%a, %d %b %Y %T %Z") + "\r\nX-Powered-By: " + this->server_name + "\r\n";
    
    this->header = "HTTP/1.1 " + resp + "\r\n" + this->header + "\r\n";
    
    return this->header;
}

/******************************************************************************************/

std::string Http::createRequestHeader(std::string filename, t_size filesize, std::string content_type, std::string host, short headtype) {
    this->header = "";
    
    if(headtype == Http::Method::GET) this->header = "GET /" + filename + " HTTP/1.1\r\n";
        else if(headtype == Http::Method::POST) this->header = "POST /" + filename + " HTTP/1.1\r\n";

    this->header += "Host: " + host + "\r\nConnection: keep-alive\r\nContent-Type: " + content_type + "\r\nContent-Length: " + std::to_string(filesize) + "\r\nCache-Control: no-cache\r\nAccept: */*\r\n";

    this->header += "\r\n";
    
    return this->header;
}

/******************************************************************************************/

t_byte * Http::createBinaryPacket(t_byte * file_bin, t_size file_size) {
    int range_start = this->range[0] > 0 ? this->range[0] : 0;
    int head_len = this->header.length();
    
    this->buffer_size = head_len + (file_size - range_start);
    
    this->buffer = (t_byte*) malloc(sizeof(t_byte) * this->buffer_size);
    
    memcpy(this->buffer, this->header.c_str(), head_len);
    
    if(file_size > 0)
        memcpy(this->buffer + head_len, file_bin + range_start, file_size - range_start);
        
    return this->buffer;
}

/******************************************************************************************

void Http::clear() {
    this->current_type      = -1;
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
    this->header = "";
    free(this->buffer);
    this->buffer = NULL;
    this->buffer_size = 0;
}

/******************************************************************************************/