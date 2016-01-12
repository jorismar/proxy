#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include "webserver.h"
#include "http.h"
#include <cstdio>
#include <thread>
#include <vector>
#include <session.h>

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

int g_http_port;
int g_udp_port;
std::string g_path;

Buffer * g_dashbuffer = new Buffer(100);

void start() {
	int i;
	std::string id;
	std::vector<Session*> sessions;
	
	/* INICIAR THREAD DO DASHCAST */
	
	/* INICIO DO LAÇO */
		/* RECEBIMENTO DO COMANDO */
	
		/* IF INICIO - Abrir nova sessão */
			//sessions.push_back(new Session(id, g_udp_port++, 10, g_http_port++, g_path + "/" + id));
			sessions.push_back(new Session(id, g_udp_port++, 31, g_http_port++, "./site"));
			
			while(!sessions.back()->bindUdpPort())
				sessions.back()->setUdpPort(g_udp_port++);
			
			while(!sessions.back()->bindHttpPort()) 
				sessions.back()->setHttpPort(g_http_port++);
			
			std::thread session([=](){sessions.back()->start(); return 1;});
			session.join();
			//session.detach();
			/* Responder ao Arthron enviando as Portas abertas */
		/* FIM IF */
		
		
		/* IF INICIO - Fechar sessao *
			for(i = 0; i < sessions.size(); i++)
				if(id.compare(sessions.at(i)->getID()))
					break;
		
			sessions.at(i)->stop();
			sessions.at(i)->~Session();
			sessions.erase(i);
		/* FIM IF */
		
		/* ELSE COMANDO INCORRETO*/
	/* FIM DO LAÇO */
}

int main() {
	g_http_port = 8080;
	g_udp_port = 1234;
	
	start();

    return 0;
}