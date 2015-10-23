#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "dash.h"
#include <cstdio>
#include <thread>    
#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234
    
int main() {
	
    GF_ISOFile *file;
    Dash dash(file);  

    return 0;
}

