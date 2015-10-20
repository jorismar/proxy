#include <stdlib.h>
#include "types.h"

typedef struct tDataPacket{
    unsigned int size;
    byte * data;
    
    byte * (*getData)();
    unsigned int (*copyByRange)(tDataPacket*, unsigned int, unsigned int);
    unsigned int (*length)();
} tDataPacket;

tDataPacket * newPacket(int);
byte * get_data();
unsigned int copy_by_range(tDataPacket*, unsigned int, unsigned int);
unsigned int length();
