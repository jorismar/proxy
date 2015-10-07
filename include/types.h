#include <cstdlib>  //exit();
#include <iostream> //cout

#define EXIT_IF(cond, msg) \
    if ((cond)) { \
    std::cout << msg << std::endl; \
    exit(1); \
}

#define PRINT(msg) std::cout << msg << std::endl;

typedef char t_byte;
