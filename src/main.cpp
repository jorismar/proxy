#include "http.h"
#include "session.h"
#include "dash.h"
#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>

/******************************************************************************************/

Socket*     g_server;
int 		g_server_port 		= 8080;
int 		g_initial_tcp_port 	= 8090;
int 		g_initial_udp_port 	= 1234;
int 		g_dash_profile 		= 1; 				// 0 = live, 1 = on demand
int 		g_controller_port 	= 8085;
std::string g_server_ip 		= "127.0.0.1";
std::string g_site_path 		= "./www";
std::string g_dash_path 		= "./dash";
std::string g_mpd_name			= "dash.mpd";
std::string g_controller_ip 	= "127.0.0.1";
std::string g_controller_url_path = "ArthronRest/api/dash_sessions";

/******************************************************************************************/

void registerOnController();
void startServer();
int findSession(std::vector<Session*>&, std::string);
std::string getJSONValue(std::string, std::string);

/******************************************************************************************/

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
                EXIT_IF(true, "[ERROR] Invalid port entry.");
            }
		} else if(!arg.compare("-initial-tcp-port")) {
			arg = argv[++i];

				try {
					g_initial_tcp_port = std::stoi(arg, &sz);
				} catch(...) {
					EXIT_IF(true, "[ERROR] Invalid initial tcp port entry.");
				}
		} else if(!arg.compare("-initial-udp-port")) {
			arg = argv[++i];

            try {
                g_initial_udp_port = std::stoi(arg, &sz);
            } catch(...) {
                EXIT_IF(true, "[ERROR] Invalid initial udp port entry.");
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
                EXIT_IF(true, "[ERROR] Invalid port format.");
            }
		} else if(!arg.compare("-controller-url-path")) {
			g_controller_url_path = argv[++i];
		} else if(!arg.compare("-h") || !arg.compare("--help")) {
			arg = argv[0];
			
			PRINT(
                "Usage: " + arg + " [config options]\n\n\
                    \r  -ip <ip:str>                  IP of server machine\n\
                    \r  -port <port:int>              TCP port used to connect proxy server with\n\
                    \r                                controller server\n\
                    \r  -initial-tcp-port <port:int>  Select the initial port used to new sessions\n\
                    \r  -initial-udp-port <port:int>  Select the initial port used to new sessions\n\
                    \r  -site-path <directory:str>    Location of directory of the site files\n\
                    \r  -live or -on-demand           Selects the profile dash to be used\n\
                    \r  -mpd <name:str>               Define the MPD name.\n\
                    \r  -dash-path <directory:str>    Location of directory of the dash files used in\n\
                    \r                                on-demand profile\n\
                    \r  -controller-ip <ip:str>       IP of the session controllers\n\
                    \r  -controller-port <port:int>   Port of the session controllers\n\
                    \r  -controller-url-path <url-path:str>  The path of the controller used to\n\
                    \r                                recongnize the server mensage\n\n\
                    \rDefault settings:\n"              << \
                    "\n\r  Server IP: \""               << g_server_ip          << "\""\
                    "\n\r  Server Port: "               << g_server_port        << \
                    "\n\r  Initial HTTP Session Port: " << g_initial_tcp_port   << \
                    "\n\r  Initial UDP Session Port: "  << g_initial_udp_port   << \
                    "\n\r  Site Directory: \""          << g_site_path          << "\"" << \
                    "\n\r  Dash Profile: "              << (g_dash_profile == Dash::Profile::LIVE ? "live" : g_dash_profile == Dash::Profile::ON_DEMAND ? "on-demand" : "Invalid") << \
                    "\n\r  MPD Name: "                  << g_mpd_name << \
                    "\n\r  Dash Directory: \""          << g_dash_path          << "\"" << \
                    "\n\r  Controller Server IP: \""    << g_controller_ip      << "\""\
                    "\n\r  Controller Server Port: "    << g_controller_port    << \
                    "\n\r  Controller URL Path: \""     << g_controller_url_path << "\"\n"\
            );
			
			return 0;
		} else {
			EXIT_IF(true, "[ERROR] Invalid Parameter: " << argv[i]);
		}
	}

	PRINT(
        "\nStarting with settings:\n"             << \
            "\n\r  Server IP: \""               << g_server_ip          << "\""\
            "\n\r  Server Port: "               << g_server_port        << \
            "\n\r  Initial HTTP Session Port: " << g_initial_tcp_port   << \
            "\n\r  Initial UDP Session Port: "  << g_initial_udp_port   << \
            "\n\r  Site Directory: \""          << g_site_path          << "\"" << \
            "\n\r  Dash Profile: "              << (g_dash_profile == Dash::Profile::LIVE ? "live" : g_dash_profile == Dash::Profile::ON_DEMAND ? "on-demand" : "Invalid") << \
            "\n\r  MPD Name: "                  << g_mpd_name << \
            "\n\r  Dash Directory: \""          << g_dash_path          << "\"" << \
            "\n\r  Controller Server IP: \""    << g_controller_ip      << "\""\
            "\n\r  Controller Server Port: "    << g_controller_port    << \
            "\n\r  Controller URL Path: \""     << g_controller_url_path << "\"\n"\
    );
	
	registerOnController();
	startServer();
	
    return 0;
}

/******************************************************************************************/

void registerOnController() {
	Http * protocol = new Http();
	Socket * socket = new Socket();
	
    /*** Connecting to controller server ***/
	while(socket->Connect(g_controller_ip, g_controller_port) < 0) {
		std::this_thread::sleep_for (std::chrono::milliseconds(1000));
	}
	
	PRINT("[INFO] Connected to the controller.");
	
	/*** selecting server port ***/
	g_server = new Socket(g_server_port);
	
	while(g_server->Bind() < 0 || g_server->Listen(10) < 0) {
		g_server->setPort(++g_server_port);
		PRINT("[INFO] Server port changed to " << g_server_port);
	}
	
	/*** preparing register message ***/
	std::string json = "{\"session_id\" : \"init\", \"ip\" : \"" + g_server_ip + "\", \"port\" : \"" + std::to_string(g_server_port) + "\"}";
	
	std::string header = protocol->genRequest(g_controller_url_path, (t_size) json.length(), Http::ContentType::JSON, "*/*", g_controller_ip + ":" + std::to_string(g_controller_port), Http::Method::POST);
	
	int size = header.length() + json.length();
	
	t_byte * buffer = (t_byte*) malloc(sizeof(t_byte) * size);
	
	memcpy(buffer, header.c_str(), header.length());
	memcpy(buffer + header.length(), json.c_str(), json.length());
	
	/*** sending request message ***/
	EXIT_IF(socket->Send(buffer, size) < 0, "");

	free(buffer);
	
	PRINT("[INFO] Sending register request...");
	
	buffer = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
	memset(buffer, 0, Http::BufferSize::MAX);

	/*** checking if request was accepted ***/
	EXIT_IF(socket->Receive(buffer, Http::BufferSize::MAX, 30) < 0, "");
	
	PRINT("[INFO] Processing response...");

	protocol->processResponse(buffer);

	free(buffer);
	socket->Close();

	//EXIT_IF(protocol->get_reply_status() != Http::Status::CREATED, "[ERROR] The controller server responded with status code " << protocol->get_reply_status());
}

/******************************************************************************************/

void startServer() {
	int head_size, json_size, packet_size;
	std::vector<Session*> sessions;
	Http * protocol = new Http();
	std::string header, id, json;
	bool isjson;
	t_byte * rcv_packet = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
	t_byte * snd_packet;
	t_socket client;

	PRINT("[INFO] Server started! Running on port: " << g_server->getPort());
	
	while(true) {
		id = "";
		json = "";

		client = g_server->Accept();

		Socket::readFrom(client, rcv_packet, Http::BufferSize::MAX, 30);
		
		protocol->processRequest(rcv_packet);
		
		isjson = protocol->get_content_type() == Http::ContentType::JSON;
		
		if(isjson) {
			std::string aux(rcv_packet);
			bool accept = true;

			try {
				json = aux.substr(aux.find("\r\n\r\n") + 4,  aux.length() - 1);
			
				id = getJSONValue("session_id", json);

				if(id.length() > 0) {
					aux = getJSONValue("command", json);
					json = "";
					
					if(aux.length() > 0) {
						if(aux.find("open") != std::string::npos) {
							PRINT("[INFO] Command received: Start Session id:" << id);
							
							if(findSession(sessions, id) < 0) {
								sessions.push_back(new Session(id, g_initial_udp_port++, 1000, g_initial_tcp_port++, g_site_path, g_dash_profile, g_dash_path, g_mpd_name));
								
								while(!sessions.back()->bindUdpPort())
									sessions.back()->setUdpPort(g_initial_udp_port++);
								
								while(!sessions.back()->bindHttpPort()) 
									sessions.back()->setHttpPort(g_initial_tcp_port++);
								
								std::thread session([=](){sessions.back()->start(); return 1;});
								session.detach();
								
								json = "{\"ip\":\"" + g_server_ip + "\",\n\"udp\":\"" + std::to_string(sessions.back()->getUdpPort()) + "\",\n\"http\":\"" + std::to_string(sessions.back()->getHttpPort()) + "\", \"mpd_name\" : \"" + g_mpd_name + "\"}";
							} else {
								PRINT("\n[ERROR] Failed to open the Session " + id + ". The session already running.");
								accept = false;
							}
						} else if(aux.find("close") != std::string::npos) {
							PRINT("[INFO] Command received: Close Session id:" << id);
								
							int i = findSession(sessions, id);
								
							if(i >= 0) {
								sessions.at(i)->stop();
								sessions.at(i)->~Session();
								sessions.erase(sessions.begin() + i);
							} else {
								PRINT("\n[ERROR] Failed to close the session " + id + ". The session does not exist.");
								accept = false;
							}
						} else {
							PRINT("\n[ERROR] Invalid JSON received. Unrecognized command.");
							accept = false;
						}
					} else {
						PRINT("\n[ERROR] Invalid JSON received. Command not found.");
						accept = false;
					}
				} else {
					PRINT("\n[ERROR] Invalid JSON received. Session ID not found.");
					accept = false;
				}
			} catch(...) {
				accept = false;
                PRINT("\n[ERROR] Invalid JSON received.");
            }
			
			if(accept) header = protocol->genResponse(json.length(), "json", "", Http::Status::OK);
                else header = protocol->genResponse(0, "", "", Http::Status::BAD_REQUEST);
		} else {
            PRINT("\n[ERROR] JSON not found.");
            header = protocol->genResponse(0, "", "", Http::Status::NOT_FOUND);
        }

		head_size = header.length();
		json_size = json.length();
		packet_size = head_size + json_size;
		
		snd_packet = (t_byte*) malloc(sizeof(t_byte) * packet_size);
		
		memcpy(snd_packet, header.c_str(), head_size);
		
		if(isjson)
			memcpy(snd_packet + head_size, json.c_str(), json_size);
			
		Socket::sendTo(client, snd_packet, packet_size);

		Socket::Close(client);

		memset(rcv_packet, 0, Http::BufferSize::MAX);
		free(snd_packet);
	}

	free(rcv_packet);
}

/******************************************************************************************/

int findSession(std::vector<Session*> &list, std::string id) {
	int i;
	
	for(i = 0; i < list.size(); i++) {
		if(!id.compare(list.at(i)->getID()))
			break;
	}
	
	i = i < list.size() ? i : -1;
	
	return i;
}

/******************************************************************************************/

std::string getJSONValue(std::string field, std::string src) {
	std::string value = "";
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
						value += src.at(i);
					}
					
					break;
				}
			} else if(src.at(i) != ' ')
				break;
		}
	}

	return value;
}

