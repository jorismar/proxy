#include <cstdlib>  //exit();
#include <iostream> //cout
#include <vector>

#define EXIT_IF(cond, msg) \
    if ((cond)) { \
        std::cout << msg << std::endl << "Close." << std::endl; \
        exit(1); \
    }

#define PRINT(msg) std::cout << msg << std::endl;

typedef char t_byte;
typedef int t_socket;
typedef unsigned long long t_size;
typedef unsigned long long t_pos;

