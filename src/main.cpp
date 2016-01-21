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

std::string g_server_ip = "150.165.205.84";
int g_server_port = 8080;


std::string g_init_path = "ArthronRest/api/dash_sessions";
std::string g_init_ip = "150.165.205.159";
int g_init_port = 8080;

Buffer * g_dashbuffer = new Buffer(100);
Socket * server = new Socket(g_server_port++);

void signIn(std::string ip, int port) {
	Http * header = new Http();
	Socket * socket = new Socket();
	t_byte * buffer;
	std::string request;
	int size;
	
	EXIT_IF(socket->Connect(ip, port) < 0, "");
	
	while(server->Bind() < 0 || server->Listen(10) < 0)
		server->setPort(g_server_port++);
	
	std::string json_msg = "{\n\t\"session_id\" : \"init\",\n\t\"ip\" : \"" + g_server_ip + "\",\n\t\"port\" : \"" + std::to_string(g_server_port - 1) + "\"\n}";
	
	request = header->genRequest(g_init_path, (t_size) json_msg.length(), Http::ContentType::JSON, "*/*", ip + ":" + std::to_string(port), Http::Method::POST);
	
	size = request.length() + json_msg.length();
	
	buffer = (t_byte*) malloc(sizeof(t_byte) * size);
	
	memcpy(buffer, request.c_str(), request.length());
	memcpy(buffer + request.length(), json_msg.c_str(), json_msg.length());
	
	EXIT_IF(socket->Send(buffer, size) < 0, "");

	memset(buffer, 0, size);

	EXIT_IF(socket->Receive(buffer, size, 30) < 0, "");
	
	header->clear();

	header->processResponse(buffer);
	
	memset(buffer, 0, size);
	free(buffer);

	EXIT_IF(header->get_reply_status() != Http::Status::CREATED, "Error: The server responded with status code " << header->get_reply_status() << ".");
	
	socket->Close();
}

std::string getValue(std::string field, std::string src, std::string assign_operator) {
	std::string ret = "";
	int i, pos;
	
	pos = src.find("\"" + field + "\"");
	
	if(pos != std::string::npos)
		pos = src.find(assign_operator, pos + field.length() + 2);
	
	if(pos != std::string::npos)
		pos = src.find("\"", pos + assign_operator.length());
	
	if(pos != std::string::npos) {
		for(i = pos + 1; i < src.length() && src.at(i) != '\"'; i++) {
			ret += src.at(i);

			if(src.at(i) == ',' && src.at(i) == '\n' && src.at(i) == '}') {
				ret = "";
				break;
			}
		}
	}
	
	return ret;	
}

void start() {
	int i, head_size, json_size, packet_size;
	int g_http_port = g_server_port;
	int g_udp_port = 1234;
	std::vector<Session*> sessions;
	t_socket client;
	Http * header = new Http();
	std::string resp_header, filename, id, json_msg;
	bool is_json;
	bool start;
	t_byte * rcv_packet = (t_byte*) malloc(sizeof(t_byte) * 1024);
	t_byte * snd_packet;

	PRINT("Server running on port: " << server->getPort());
	
	while(true) {
		id = "";
		json_msg = "";

		client = server->Accept();
		
		if(client < 0)
			PRINT("ERROR ON ACCEPT")

		Socket::readFrom(client, rcv_packet, 1024);
		
		PRINT(rcv_packet)
		
		header->processRequest(rcv_packet);
		
		//filename = header->get_reqsted_file();
		
		//is_json = !filename.substr(filename.rfind(".", filename.length() - 1) + 1, filename.length()).compare("json");
		
		is_json = header->get_content_type() == Http::ContentType::JSON ? true : false;
		
		if(is_json) {
			std::string aux(rcv_packet);
			bool accept = true;
			json_msg = aux.substr(aux.find("\r\n\r\n") + 4,  aux.length() - 1);
			
			id = getValue("session_id", json_msg, ":");

			if(id.length() > 0) {
				aux = getValue("command", json_msg, ":");
				
				if(aux.length() > 0) {
					if(aux.find("open") != std::string::npos) {
						PRINT("Command received: Start session: " << id);
						
						sessions.push_back(new Session(id, g_udp_port++, 33, g_http_port++, "/home/jorismar/proxy/site"));
						
						while(!sessions.back()->bindUdpPort())
							sessions.back()->setUdpPort(g_udp_port++);
						
						while(!sessions.back()->bindHttpPort()) 
							sessions.back()->setHttpPort(g_http_port++);
						
						std::thread session([=](){sessions.back()->start(); return 1;});
						session.detach();
						
						json_msg = "{\n\"ip\":\"" + g_server_ip + "\",\n\"udp\":\"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\":\"" + std::to_string(sessions.back()->getHttpPort()) + "\"\n}";
					} else if(aux.find("close") != std::string::npos) {
						PRINT("Command received: Close session: " << id);
						
						for(i = 0; i < sessions.size(); i++)
							if(!id.compare(sessions.at(i)->getID()))
								break;
							
						if(i < sessions.size()) {
							sessions.at(i)->stop();
							//sessions.at(i)->~Session();
							sessions.erase(sessions.begin() + i);
							
							json_msg = "";
						} else {
							PRINT("Error: session id not found.");
							json_msg = "";
						}
					} else accept = false;
				} else accept = false;
			} else accept = false;
			
			if(accept) {
				resp_header = header->genResponse(json_msg.length(), "json", "", Http::Status::OK);
			} else {
				PRINT("Command received: Invalid");
				json_msg = "";
				resp_header = header->genResponse(json_msg.length(), "json", "", Http::Status::BAD_REQUEST);
			}
		} else resp_header = header->genResponse(0, "", "", Http::Status::NOT_FOUND);

		head_size = resp_header.length();
		json_size = json_msg.length();
		packet_size = head_size + json_size;
		
		snd_packet = (t_byte*) malloc(sizeof(t_byte) * packet_size);
		
		memcpy(snd_packet, resp_header.c_str(), head_size);
		
		if(is_json)
			memcpy(snd_packet + head_size, json_msg.c_str(), json_size);
			
		Socket::sendTo(client, snd_packet, packet_size);
		
		Socket::Close(client);

		memset(rcv_packet, 0, 1024);
		free(snd_packet);
	}//20161006528620

	free(rcv_packet);
}

int main(int argc, char *argv[]) {
	signIn(g_init_ip, g_init_port);
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
