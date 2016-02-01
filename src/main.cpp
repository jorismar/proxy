#include "buffer.h"
#include "datapacket.h"
#include "datagramsocket.h"
#include "mpegts.h"
#include "webserver.h"
#include "http.h"
#include "dash.h"
#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>
#include <session.h>

std::string g_server_ip 		= "127.0.0.1";
int 		g_server_port 		= 8080;
int 		g_initial_tcp_port 	= 8090;
int 		g_initial_udp_port 	= 1234;
std::string g_site_path 		= "./site";
int 		g_dash_profile 		= 1; 				// 0 = live, 1 = on demand
std::string g_dash_path 		= "./dash";
std::string g_mpd_name			= "dash.mpd";
std::string g_controller_ip 	= "127.0.0.1";
int 		g_controller_port 	= 8080;
std::string g_controller_url_path = "ArthronRest/api/dash_sessions";

Socket * server;

void registerOnController();
void start();
int find(std::vector<Session*>&, std::string);
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
                g_server_port = std::stoi(arg, &sz);
            } catch(...) {
                std::cout << "Invalid port entry." << std::endl;
            }
		} else if(!arg.compare("-initial-tcp-port")) {
			arg = argv[++i];

				try {
					g_initial_tcp_port = std::stoi(arg, &sz);
				} catch(...) {
					std::cout << "Invalid initial tcp port entry." << std::endl;
				}
		} else if(!arg.compare("-initial-udp-port")) {
			arg = argv[++i];

            try {
                g_initial_udp_port = std::stoi(arg, &sz);
            } catch(...) {
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
                g_controller_port = std::stoi(arg, &sz);
            } catch(...) {
                std::cout << "Invalid port format." << std::endl;
            }
		} else if(!arg.compare("-controller-url-path")) {
			g_controller_url_path = argv[++i];
		} else if(!arg.compare("-h") || !arg.compare("--help")) {
			arg = argv[0];
			
			PRINT(
                "Usage: " + arg + " [config options]\n\
                    \n\r  -ip <ip:str>                  IP of dashproxy server\
                    \n\r  -port <port:int>              TCP port of proxy server used to connect with\
                    \n\r                                controller server\
                    \n\r  -initial-tcp-port <port:int>  Select the initial port used to new sessions\
                    \n\r  -initial-udp-port <port:int>  Select the initial port used to new sessions\
                    \n\r  -site-path <directory:str>    Location of directory of the site files\
                    \n\r  -live or -on-demand           Selects the profile dash to be used\
                    \n\r  -mpd <name:str>               Define the MPD name.\
                    \n\r  -dash-path <directory:str>    Location of directory of the dash files used in\
                    \n\r                                on-demand profile\
                    \n\r  -controller-ip <ip:str>       IP of the session controllers\
                    \n\r  -controller-port <port:int>   Port of the session controllers\
                    \n\r  -controller-url-path <url-path:str>  The path of the controller used to\
                    \n\r                                recongnize the server mensage\n"\
            );
			
			return 0;
		} else {
			std::cout << "Invalid Parameter: " << argv[i] << std::endl;
			return 1;
		}
	}

	PRINT(
        "Starting with settings:\n"             << \
            "\n\r  Server IP: "                 << g_server_ip          << \
            "\n\r  Server Port: "               << g_server_port        << \
            "\n\r  Initial HTTP Session Port: " << g_initial_tcp_port   << \
            "\n\r  Initial UDP Session Port: "  << g_initial_udp_port   << \
            "\n\r  Site Directory: \""          << g_site_path          << "\"" << \
            "\n\r  Dash Profile: "              << (g_dash_profile == Dash::Profile::LIVE ? "live" : g_dash_profile == Dash::Profile::ON_DEMAND ? "on-demand" : "Invalid") << \
            "\n\r  MPD Name: "                  << g_mpd_name << \
            "\n\r  Dash Directory: \""          << g_dash_path          << "\"" << \
            "\n\r  Controller Server IP: "      << g_controller_ip      << \
            "\n\r  Controller Server Port: "    << g_controller_port    << \
            "\n\r  Controller URL Path: \""     << g_controller_url_path << "\"\n"\
    );
	
	registerOnController();
	start();
	
    return 0;
}

void registerOnController() {
	Http * protocol = new Http();
	Socket * socket = new Socket();
	
	while(socket->Connect(g_controller_ip, g_controller_port) < 0) {
		std::this_thread::sleep_for (std::chrono::milliseconds(1000));
	}
	
	PRINT("Connected to the controller.");
	
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

	PRINT("Request start sent.");
	
	memset(buffer, 0, size);

	/* receiving response */
	EXIT_IF(socket->Receive(buffer, size, 30) < 0, "");
	
	PRINT("Processing response...");

	/* checking if request was accepted */
	protocol->processResponse(buffer);

	free(buffer);
	socket->Close();

	EXIT_IF(protocol->get_reply_status() != Http::Status::CREATED, "[ERROR] The server responded with status code " << protocol->get_reply_status());
}

void start() {
	int head_size, json_size, packet_size;
	std::vector<Session*> sessions;
	Http * protocol = new Http();
	std::string header, id, json;
	bool isjson;
	t_byte * rcv_packet = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
	t_byte * snd_packet;
	t_socket client;

	PRINT("Server started!\nRunning on port: " << server->getPort());
	
	while(true) {
		id = "";
		json = "";

		client = server->Accept();

		Socket::readFrom(client, rcv_packet, Http::BufferSize::MAX, 0);
		
		protocol->processRequest(rcv_packet);
		
		isjson = protocol->get_content_type() == Http::ContentType::JSON;
		
		if(isjson) {
			std::string aux(rcv_packet);
			bool accept = true;
			json = aux.substr(aux.find("\r\n\r\n") + 4,  aux.length() - 1);
			
			id = getValue("session_id", json, ":");

			if(id.length() > 0) {
				aux = getValue("command", json, ":");
				json = "";
                
				if(aux.length() > 0) {
					if(aux.find("open") != std::string::npos) {
						PRINT("Command received: Start " << id);
						
						if(find(sessions, id) < 0) {
							sessions.push_back(new Session(id, g_initial_udp_port++, 1000, g_initial_tcp_port++, g_site_path, g_dash_profile, g_dash_path, g_mpd_name));
							
							while(!sessions.back()->bindUdpPort())
								sessions.back()->setUdpPort(g_initial_udp_port++);
							
							while(!sessions.back()->bindHttpPort()) 
								sessions.back()->setHttpPort(g_initial_tcp_port++);
							
							std::thread session([=](){sessions.back()->start(); return 1;});
							session.detach();
							
							json = "{\"ip\":\"" + g_server_ip + "\",\n\"udp\":\"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\":\"" + std::to_string(sessions.back()->getHttpPort()) + "\", \"mpd_name\" : \"" + g_mpd_name + "\"}";
						} else {
							PRINT("[ERROR] Failed to open the Session " + id + ". The session already running.");
							accept = false;
						}
					} else if(aux.find("close") != std::string::npos) {
						PRINT("Command received: Close " << id);
							
						int i = find(sessions, id);
							
						if(i >= 0) {
							sessions.at(i)->stop();
							//sessions.at(i)->~Session();
							sessions.erase(sessions.begin() + i);
						} else {
							PRINT("[ERROR] Failed to close the session " + id + ". The session does not exist.");
							accept = false;
						}
					} else {
						PRINT("[ERROR] Invalid JSON received. Unrecognized command.");
						accept = false;
					}
				} else {
					PRINT("[ERROR] Invalid JSON received. Command not found.");
					accept = false;
				}
			} else {
				PRINT("[ERROR] Invalid JSON received. Session ID not found.");
				accept = false;
			}
			
			if(accept) header = protocol->genResponse(json.length(), "json", "", Http::Status::OK);
                else header = protocol->genResponse(0, "", "", Http::Status::BAD_REQUEST);
		} else {
            PRINT("[ERROR] JSON not found.");
            header = protocol->genResponse(0, "", "", Http::Status::NOT_FOUND);
        }

		head_size = header.length();
		json_size = json.length();
		packet_size = head_size + json_size;
		
		snd_packet = (t_byte*) malloc(sizeof(t_byte) * packet_size);
		memset(rcv_packet, 0, Http::BufferSize::MAX);
		
		memcpy(snd_packet, header.c_str(), head_size);
		
		if(isjson)
			memcpy(snd_packet + head_size, json.c_str(), json_size);
			
		Socket::sendTo(client, snd_packet, packet_size);

		server->CloseClient();

		memset(rcv_packet, 0, Http::BufferSize::MAX);
		free(snd_packet);
	}

	free(rcv_packet);
}

int find(std::vector<Session*> &list, std::string id) {
	int i;
	
	for(i = 0; i < list.size(); i++) {
		if(!id.compare(list.at(i)->getID()))
			break;
	}
	
	i = i < list.size() ? i : -1;
	
	return i;
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

