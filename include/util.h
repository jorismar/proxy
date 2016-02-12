#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>  //exit();
#include <iostream> //cout
#include <string>
#include <ctime>    // time();
#include <signal.h>

#define EXIT_IF(cond, msg) \
    if ((cond)) { \
        std::cout << std::endl << msg << "\n\nClose." << std::endl; \
        exit(1); \
    }

#define PRINT(msg) std::cout << msg << std::endl;

typedef char t_byte;
typedef int t_socket;
typedef unsigned long long t_size;
typedef unsigned long long t_pos;

/******************************************************************************************/

static std::string getDate(std::string format) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    strftime (buffer, 80, format.c_str(), timeinfo);
    
    return std::string(buffer);
}

/******************************************************************************************/

static std::string getJSONValue(std::string field, std::string src) {
	std::string value = "";
	int i, pos;
	char c;
	
	pos = src.find("\"" + field + "\"");
	
	if(pos != std::string::npos) {
		c = ':';
		
		for(i = pos + field.length() + 2; i < src.length(); i++) {
			if(src.at(i) == c) {
				if(c == ':') {
					c = '\"';
				} else if(c == '\"') {
					for(i = i + 1; i < src.length() && src.at(i) != '\"'; i++) {
						value += src.at(i);
					}
					
					break;
				}
			} else if(src.at(i) != ' ')
				break;
		}
	}

	return value;
}

#endif


/*
http://www.anirudhtom.com/2011/03/live-streaming-video-tutorial-for.html
http://download.tsi.telecom-paristech.fr/gpac/doc/libgpac/mpegts_8h.html#structtag__m2ts__es
https://pt.wikipedia.org/wiki/Signal.h
http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
http://www.gdsw.at/languages/c/programming-bbrown/c_075.htm
http://www.cplusplus.com/reference/cstdio/setbuf/
http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
http://www.cplusplus.com/reference/ctime/


*/
