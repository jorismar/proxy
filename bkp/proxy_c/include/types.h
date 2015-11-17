#include <stdio.h>  //exit();
#include <stdlib.h>

#define EXIT_IF(cond, msg) \
    if ((cond)) { \
    printf("%s\n", msg); \
    exit(1); \
}

#define PRINT(msg) printf("%s\n", msg);

typedef char byte;
