#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include "webserver.h"
#include "http.h"
#include <cstdio>
#include <thread>

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

Buffer * filebuffer = new Buffer(100);

void webserver() {
    Webserver * svr = new Webserver(8080, &filebuffer, "/home/jorismar/proxy/bkp/site");
    svr->start();
}

void file_generator() {
/*	int count;
	char * bin;
	FILE * pfile;
	
	pfile = fopen("/home/jorismar/proxy/bkp/site/video.mp4", "rb");
	
    EXIT_IF(pfile == NULL, "Erro ao abrir arquivo.");
    
    fseek (pfile , 0, SEEK_END);
    long fsize = ftell (pfile);
    rewind (pfile);
	
    bin = (char*) malloc (sizeof(char) * fsize);

	count = fread (bin, 1, fsize, pfile);

	filebuffer->add(new VirtualFile("video.mp4", TYPE_MP4, Http::getDate(), bin, fsize));
	
	fclose(pfile);*/
}

int main() {
    std::thread svr(webserver);
	std::thread filegen(file_generator);
	
	filegen.detach();
    svr.join();

    return 0;
}