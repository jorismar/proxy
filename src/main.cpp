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
std::string ip = "127.0.0.1";

Buffer * g_dashbuffer = new Buffer(100);

void start() {
	int i, json_msg_size;
	std::vector<Session*> sessions;
	t_socket client;
	Socket * socket = new Socket(g_http_port++);
	Http * header = new Http();
	std::string resp_header, filename, id, json_msg;
	bool is_json;
	bool start;
	t_byte * packet;

	EXIT_IF(socket->Bind() < 0 || socket->Listen(10) < 0, "")

	PRINT("Server running on port: " << socket->getPort());
	
	while(true) {
		id = "";
		json_msg = "";
		
	    packet = (t_byte*) malloc(sizeof(t_byte) * 1024);

		client = socket->Accept();
		
		EXIT_IF(client < 0, "")

		Socket::readFrom(client, packet, 1024);
		
		PRINT(packet)
		
		header->process(packet);
		
		filename = header->get_reqsted_file();
		
		is_json = !filename.substr(filename.rfind(".", filename.length() - 1) + 1, filename.length()).compare("json");
		
		if(is_json) {
			std::string aux(packet);
			json_msg = aux.substr(aux.find("\r\n\r\n", aux.length() - 1) + 4,  aux.length());
			
			PRINT(json_msg);
			
			if(json_msg.find("\"command\" : \"open\"", json_msg.length() - 1) != std::string::npos) {
				aux = "\"session_id\" : \"";
				
				for(i = json_msg.find(aux, json_msg.length() - 1) + aux.length(); i < json_msg.length() && json_msg.at(i) != '\"'; i++)
					id += json_msg.at(i);
				
				//sessions.push_back(new Session(id, g_udp_port++, 10, g_http_port++, g_path + "/" + id));
				sessions.push_back(new Session(id, g_udp_port++, 31, g_http_port++, "./site"));
				
				while(!sessions.back()->bindUdpPort())
					sessions.back()->setUdpPort(g_udp_port++);
				
				while(!sessions.back()->bindHttpPort()) 
					sessions.back()->setHttpPort(g_http_port++);
				
				std::thread session([=](){sessions.back()->start(); return 1;});
				session.detach();
				
				json_msg = "";
				
				json_msg = json_msg + "{\n\"response\" : \"ok\",\n\"ip\" : \"" + ip + "\",\n\"udp\" : \"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\" : \"" + std::to_string(sessions.back()->getHttpPort()) + "\"\n}";
			} else if(json_msg.find("\"command\" : \"close\"", json_msg.length() - 1) != std::string::npos) {
				aux = "\"session_id\" : \"";
				
				for(i = json_msg.find(aux, json_msg.length() - 1) + aux.length(); i < json_msg.length() && json_msg.at(i) != '\"'; i++)
					id += json_msg.at(i);
				
				for(i = 0; i < sessions.size(); i++) {
					if(id.compare(sessions.at(i)->getID())) {
						sessions.at(i)->stop();
						sessions.at(i)->~Session();
						sessions.erase(sessions.begin() + i);
					}
				}
				
				json_msg = "{\n\"response\" : \"ok\"\n}";
				json_msg_size = json_msg.length();
			} else {
				json_msg = "{\n\"response\" : \"error\"\n}";
			}
			
			resp_header = header->generate(json_msg.length(), "json", "");
		} else resp_header = header->generate(0, "", "");

		free(packet);

		packet = (t_byte*) malloc(sizeof(t_byte) * (resp_header.length() + json_msg.length()));
		
		memcpy(packet, resp_header.c_str(), resp_header.length());
		
		if(is_json)
			memcpy(packet + resp_header.length(), json_msg.c_str(), json_msg.length());
			
		Socket::sendTo(client, packet, resp_header.length() + json_msg.length());
		
		Socket::Close(client);

		free(packet);
	}//20161006528620
}

int main() {
	g_http_port = 8080;
	g_udp_port = 1234;
	
	start();

    return 0;
}