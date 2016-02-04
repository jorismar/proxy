#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>  //exit();
#include <iostream> //cout
#include <string>
#include <ctime>    // time();

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

static std::string getDate(std::string format) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    strftime (buffer, 80, format.c_str(), timeinfo);
    
    return std::string(buffer);
}

#endif
