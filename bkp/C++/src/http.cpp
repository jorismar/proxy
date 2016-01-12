#include "http.h"

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
    this->server_name = "";
}

Http::~Http() {
    
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


void Http::read_msg(t_byte* msg) {
    // IMPLEMENTAR
}

t_byte* Http::write_msg(int reply_status, int connection_state) {
    // IMPLEMENTAR
}
