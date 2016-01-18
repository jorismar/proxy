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
std::string server_ip = "127.0.0.1";
int server_port = 8080;
std::string arthron_ip = "";
int arthron_port = 99999;

Buffer * g_dashbuffer = new Buffer(100);

bool signIn(std::string ip, int port) {
	Http * header = new Http();
	Socket * socket = new Socket();
	t_byte * buffer;
	std::string request;
	
	EXIT_IF(socket->Connect(ip, port) < 0, "");
	
	std::string json_msg = "{\n\"command\" : \"connect\",\n\"ip\" : \"" + server_ip + "\",\n\"port\" : \"" + std::to_string(server_port) + "\"\n}";
	
	request = header->genRequest("", (t_size) json_msg.length(), "*/*", ip + ":" + std::to_string(port), Http::Type::POST);
	
	buffer = (t_byte*) malloc(sizeof(t_byte) * (request.length() + json_msg.length()));
	
	memcpy(buffer, request.c_str(), request.length());
	memcpy(buffer + request.length(), json_msg.c_str(), json_msg.length());
	
	EXIT_IF(socket->Send(buffer, request.length() + json_msg.length()) < 0, "");
	
	socket->Close();
	
	// Se necessário, implementar processamento de mensagem de confirmação
	
	return true;
}

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
		
		header->processRequest(packet);
		
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
				
				json_msg = json_msg + "{\n\"response\" : \"ok\",\n\"ip\" : \"" + server_ip + "\",\n\"udp\" : \"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\" : \"" + std::to_string(sessions.back()->getHttpPort()) + "\"\n}";
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
			
			resp_header = header->genResponse(json_msg.length(), "json", "");
		} else resp_header = header->genResponse(0, "", "");

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

int main(int argc, char *argv[]) {
	g_http_port = server_port;
	g_udp_port = 1234;
	
	if(signIn(arthron_ip, arthron_port))
		start();

    return 0;
}

/*
POST /file.json HTTP/1.1
Host: 192.168.15.128:8080
Connection: keep-alive
Content-Length: 0
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36
Cache-Control: no-cache
Origin: chrome-extension://mkhojklkhkdaghjjfdnphfphiaiohkef
Accept: **
DNT: 1
Accept-Encoding: gzip, deflate
Accept-Language: pt-BR,pt;q=0.8,en-US;q=0.6,en;q=0.4
*/
