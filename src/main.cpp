#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include "webserver.h"
#include "http.h"
#include "dash.h"
#include <cstdio>
#include <thread>
#include <vector>
#include <session.h>

#define PACKET_SIZE 1316
#define BUFFER_SIZE 1000
#define UDP_PORT 1234

std::string g_server_ip 		= "127.0.0.1";
int 		g_server_port 		= 8080;
int 		g_initial_tcp_port 	= 8090;
int 		g_initial_udp_port 	= 1234;
std::string g_site_path 		= "./site";
int 		g_dash_profile 		= 1; 				// 0 = live, 1 = on demand
std::string g_dash_path 		= "./dash";
std::string g_mpd_name			= "dashcast.mpd";
std::string g_controller_ip 	= "127.0.0.1";
int 		g_controller_port 	= 8080;
std::string g_controller_url_path = "ArthronRest/api/dash_sessions";

Socket * server;

void registerOnController();
void start();
std::string getValue(std::string, std::string, std::string);

int main(int argc, char *argv[]) {
	 std::string::size_type sz;
	
	for(int i = 1; i < argc; i++) {
		std::string arg(argv[i]); 
		
		if(!arg.compare("-ip")) {
			g_server_ip = argv[++i];
		} else if(!arg.compare("-port")) {
			arg = argv[++i];

			try {
				try {
					g_server_port = std::stoi(arg, &sz);
				} catch(const std::invalid_argument& ex) {
					std::cout << "Invalid port entry." << std::endl;
				}
			} catch (const std::out_of_range& ex) {
				std::cout << "Invalid port entry." << std::endl;
			}
		} else if(!arg.compare("-initial-tcp-port")) {
			arg = argv[++i];

			try {
				try {
					g_initial_tcp_port = std::stoi(arg, &sz);
				} catch(const std::invalid_argument& ex) {
					std::cout << "Invalid initial tcp port entry." << std::endl;
				}
			} catch (const std::out_of_range& ex) {
				std::cout << "Invalid initial tcp port entry." << std::endl;
			}
		} else if(!arg.compare("-initial-udp-port")) {
			arg = argv[++i];

			try {
				try {
					g_initial_udp_port = std::stoi(arg, &sz);
				} catch(const std::invalid_argument& ex) {
					std::cout << "Invalid initial udp port entry." << std::endl;
				}
			} catch (const std::out_of_range& ex) {
				std::cout << "Invalid initial udp port entry." << std::endl;
			}
		} else if(!arg.compare("-site-path")) {
			g_site_path = argv[++i];
		} else if(!arg.compare("-live")) {
				g_dash_profile = Dash::Profile::LIVE;
				g_dash_path = "";
		}else if(!arg.compare("-on-demand")) {
			g_dash_profile = Dash::Profile::ON_DEMAND;
		} else if(!arg.compare("-dash-path")) {
			g_dash_path = argv[++i];
		} else if(!arg.compare("-mpd")) {
			g_mpd_name = argv[++i];
		} else if(!arg.compare("-controller-ip")) {
			g_controller_ip = argv[++i];
		} else if(!arg.compare("-controller-port")) {
			arg = argv[++i];

			try {
				try {
					g_controller_port = std::stoi(arg, &sz);
				} catch(const std::invalid_argument& ex) {
					std::cout << "Invalid port entry." << std::endl;
				}
			} catch (const std::out_of_range& ex) {
				std::cout << "Invalid port entry." << std::endl;
			}
		} else if(!arg.compare("-controller-url-path")) {
			g_controller_url_path = argv[++i];
		} else if(!arg.compare("-h") || !arg.compare("--help")) {
			arg = argv[0];
			
			std::string str("Usage: " + arg + " [config options]\n\n");
			
			str += "  -ip <ip>                  IP of dashproxy server\n";
			str += "  -port <port>              TCP port of proxy server used to connect with\n";
			str += "                            controller server\n";
			str += "  -initial-tcp-port <port>  Select the initial port used to create new sessions\n";
			str += "  -initial-udp-port <port>  Select the initial port used to create new sessions\n";
			str += "  -site-path <directory>    Location of directory of the site files\n";
			str += "  -live or -on-demand       Selects the profile dash to be used\n";
			str += "  -mpd <name>               Define the MPD name.\n";
			str += "  -dash-path <directory>    Location of directory of the dash files (fragments,\n";
			str += "                            initializations and mpd) used in on-demand profile\n";
			str += "  -controller-ip <ip>       IP of the session controllers\n";
			str += "  -controller-port <port>   Port of the session controllers\n";
			str += "  -controller-url-path <url-path>  The path of the controller used to recongnize\n";
			str += "                            the server mensage\n";
			
			std::cout << str << std::endl;
			
			return 0;
		} else {
			std::cout << "Invalid Parameter: " << argv[i] << std::endl;
			return 1;
		}
	}

	std::cout << std::endl << "Starting with arguments:" << std::endl << std::endl;

	std::cout << "Server IP: " << g_server_ip << std::endl;
	std::cout << "Server Port: " << g_server_port << std::endl;
	std::cout << "Initial HTTP Session Port: " << g_initial_tcp_port << std::endl;
	std::cout << "Initial UDP Session Port: " << g_initial_udp_port << std::endl;
	std::cout << "Site Directory: \"" << g_site_path << "\"" << std::endl;
	std::cout << "Dash Profile: " << (g_dash_profile == Dash::Profile::LIVE ? "live" : g_dash_profile == Dash::Profile::ON_DEMAND ? "on-demand" : "Invalid") << std::endl;
	std::cout << "MPD Name: " << g_mpd_name << std::endl;
	std::cout << "Dash Directory: \"" << g_dash_path << "\"" << std::endl;
	std::cout << "Controller Server IP: " << g_controller_ip << std::endl;
	std::cout << "Controller Server Port: " << g_controller_port << std::endl;
	std::cout << "Controller URL Path: \"" << g_controller_url_path << "\"" << std::endl << std::endl;
	
	registerOnController();
	start();
	
    return 0;
}

void registerOnController() {
	Http * protocol = new Http();
	Socket * socket = new Socket();
	
	EXIT_IF(socket->Connect(g_controller_ip, g_controller_port) < 0, "");
	
	/* selecting server port */
	server = new Socket(g_server_port);
	
	while(server->Bind() < 0 || server->Listen(10) < 0) {
		server->setPort(++g_server_port);
		PRINT("Server port changed to " << g_server_port);
	}
	
	/* preparing request message */
	std::string json = "{\"session_id\" : \"init\", \"ip\" : \"" + g_server_ip + "\", \"port\" : \"" + std::to_string(g_server_port) + "\"}";
	
	std::string header = protocol->genRequest(g_controller_url_path, (t_size) json.length(), Http::ContentType::JSON, "*/*", g_controller_ip + ":" + std::to_string(g_controller_port), Http::Method::POST);
	
	int size = header.length() + json.length();
	
	t_byte * buffer = (t_byte*) malloc(sizeof(t_byte) * size);
	
	memcpy(buffer, header.c_str(), header.length());
	memcpy(buffer + header.length(), json.c_str(), json.length());
	
	/* sending request message */
	EXIT_IF(socket->Send(buffer, size) < 0, "");

	//PRINT(buffer)

	memset(buffer, 0, size);

	/* receiving response */
	EXIT_IF(socket->Receive(buffer, size, 30) < 0, "");

	/* checking if request was accepted */
	protocol->processResponse(buffer);

	//PRINT(buffer)

	free(buffer);
	socket->Close();

	EXIT_IF(protocol->get_reply_status() != Http::Status::CREATED, "Error: The server responded with status code " << protocol->get_reply_status());
}

void start() {
	int head_size, json_size, packet_size;
	std::vector<Session*> sessions;
	Http * protocol = new Http();
	std::string header, id, json;
	bool isjson;
	t_byte * rcv_packet = (t_byte*) malloc(sizeof(t_byte) * 2024);
	t_byte * snd_packet;
	t_socket client;

	PRINT("Server running on port: " << server->getPort());
	
	while(true) {
		id = "";
		json = "";

		client = server->Accept();

		Socket::readFrom(client, rcv_packet, 2024, 0);
		
		//PRINT(rcv_packet)

		protocol->processRequest(rcv_packet);
		
		isjson = protocol->get_content_type() == Http::ContentType::JSON;
		
		if(isjson) {
			std::string aux(rcv_packet);
			bool accept = true;
			json = aux.substr(aux.find("\r\n\r\n") + 4,  aux.length() - 1);
			
			id = getValue("session_id", json, ":");

			if(id.length() > 0) {
				aux = getValue("command", json, ":");
				
				if(aux.length() > 0) {
					if(aux.find("open") != std::string::npos) {
						PRINT("Command received: Start session: " << id);
						
						sessions.push_back(new Session(id, g_initial_udp_port++, 1000, g_initial_tcp_port++, g_site_path, g_dash_profile, g_dash_path));
						
						while(!sessions.back()->bindUdpPort())
							sessions.back()->setUdpPort(g_initial_udp_port++);
						
						while(!sessions.back()->bindHttpPort()) 
							sessions.back()->setHttpPort(g_initial_tcp_port++);
						
						std::thread session([=](){sessions.back()->start(); return 1;});
						session.detach();
						
						json = "{\"ip\":\"" + g_server_ip + "\",\n\"udp\":\"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\":\"" + std::to_string(sessions.back()->getHttpPort()) + "\", \"mpd_name\" : \"" + g_mpd_name + "\"}";
					} else if(aux.find("close") != std::string::npos) {
						PRINT("Command received: Close session: " << id);
						int i;
						
						for(i = 0; i < sessions.size(); i++) {
							if(!id.compare(sessions.at(i)->getID()))
								break;
						}
							
						if(i < sessions.size()) {
							sessions.at(i)->stop();
							//sessions.at(i)->~Session();
							sessions.erase(sessions.begin() + i);
							
							json = "";
						} else {
							PRINT("Error: session id not found.");
							json = "";
						}
					} else accept = false;
				} else accept = false;
			} else accept = false;
			
			if(accept) {
				header = protocol->genResponse(json.length(), "json", "", Http::Status::OK);
			} else {
				PRINT("Command received: Invalid");
				json = "";
				header = protocol->genResponse(json.length(), "json", "", Http::Status::BAD_REQUEST);
			}
		} else header = protocol->genResponse(0, "", "", Http::Status::NOT_FOUND);

		head_size = header.length();
		json_size = json.length();
		packet_size = head_size + json_size;
		
		snd_packet = (t_byte*) malloc(sizeof(t_byte) * packet_size);
		memset(rcv_packet, 0, 2024);
		
		memcpy(snd_packet, header.c_str(), head_size);
		
		if(isjson)
			memcpy(snd_packet + head_size, json.c_str(), json_size);
			
		Socket::sendTo(client, snd_packet, packet_size);
		
		//PRINT(snd_packet)

		server->CloseClient();

		memset(rcv_packet, 0, 2024);
		free(snd_packet);
	}

	free(rcv_packet);
}

std::string getValue(std::string field, std::string src, std::string assign_operator) {
	std::string ret = "";
	int i, pos;
	char c;
	
	pos = src.find("\"" + field + "\"");
	
	if(pos != std::string::npos) {
		c = ':';
		
		for(i = pos + field.length() + 2; i < src.length(); i++) {
			if(src.at(i) == c) {
				if(c == ':') {
					c = '\"';
				} else if(c == '\"') {
					for(i = i + 1; i < src.length() && src.at(i) != '\"'; i++) {
						ret += src.at(i);
					}
					
					break;
				}
			} else if(src.at(i) != ' ')
				break;
		}
	}

	return ret;
}
/*
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
}*/