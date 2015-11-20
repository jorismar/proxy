#include "http.h"

Http::Http() {
    this->start_range = 0;
    this->end_range = 0;
    this->connection = CLOSE;
    this->http_version = 0.0;
    this->reqst_file = "";
    this->referer = "";
    this->user_agent = "";
    this->accpt_encoding = "";
    this->accpt_lang = "";
    this->accpt_charset = "";
    this->date = "";
    this->cache_control = "";
    this->content_type = "";
    this->server_name = "";
}

Http::Http(std::string server_name) {
    this->start_range = 0;
    this->end_range = 0;
    this->connection = CLOSE;
    this->http_version = 0.0;
    this->reqst_file = "";
    this->referer = "";
    this->user_agent = "";
    this->accpt_encoding = "";
    this->accpt_lang = "";
    this->accpt_charset = "";
    this->date = "";
    this->cache_control = "";
    this->content_type = "";
    this->server_name = server_name;
}

Http::~Http() {
    // NOT IMPLEMENTED
}

int Http::get_range_size() {
    return (this->end_range - this->start_range) + 1;
}

int Http::get_range_initial_pos() {
    return this->start_range;
}

int Http::get_range_final_pos() {
    return this->end_range;
}

int Http::get_connection_state() {
    return this->connection;
}

double Http::get_version() {
    return this->http_version;
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

std::string Http::get_encoding() {
    return this->accpt_encoding;
}

std::string Http::get_language() {
    return this->accpt_lang;
}

std::string Http::get_charset() {
    return this->accpt_charset;
}

std::string Http::get_date() {
    return this->date;
}

std::string Http::get_cache_control() {
    return this->cache_control;
}

std::string Http::get_content_type() {
    return this->content_type;
}

std::string Http::get_server_name() {
    return this->server_name;
}

void Http::setServerName(std::string name) {
    this->server_name = name;
}

void Http::read_msg(DataPacket * msg) { // int Http::process(DataPacket * msg, Buffer * files_buffer)
    PRINT(std::endl << msg->get() << std::endl << std::endl);
    
    char * words = strtok(msg->get(), " "), * aux;
    int i;
    
    while(words != NULL) {
        //PRINT(words);
        if(!strcmp(words, "GET")) {
            words = strtok (NULL, " ");
            this->reqst_file = words;
        } else if(strstr(words, "HTTP/") != NULL) {
            // NOT IMPLEMENTED YET
            //aux = strtok(words, "/");
            //this->http_version = std::atof(words);
        } else if(!strcmp(words, "Host:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Connection:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Cache-Control:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Accept-Encoding:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "User-Agent:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Accept:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Referer:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Accept-Language:")) {
            // NOT IMPLEMENTED YET
        } else if(!strcmp(words, "Range:")) {
            // NOT IMPLEMENTED YET
        }
        
        words = strtok (NULL, " ");
    }
}

t_byte* Http::write_msg(int reply_status, int connection_state) {
    // IMPLEMENTAR
}
