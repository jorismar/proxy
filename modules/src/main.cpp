/**
 * \file 	main.cpp
 *
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#include "http.h"
#include "session.h"
#include "dashserver.h"
#include "util.h"
#include <cstdio>
#include <thread>
#include <vector>

/******************************************************************************************/

Socket*     g_server;                           // Server socket for communication with the controller server
int 		g_server_port 		= 8080;         // Server port for communication with the controller server
int 		g_initial_tcp_port 	= 8090;         // Initial TCP port for sessions of http clients
int 		g_initial_udp_port 	= 1234;         // Initial UDP port to receive the arthron video stream for sessions
int 		g_dash_profile 		= DashServer::Profile::ON_DEMAND;	// Dash profile. 0 = live, 1 = on demand (Not Implemented Yet)
int 		g_controller_port 	= 8085;         // Port used by the control server
int			g_on_the_fly_buffer_size = 0;		// Buffer size used in on-the-fly mode
bool        g_dash_on_the_fly   = false;        // MPEG-TS Demux, Dash Processer and HTTP Sender run in on-the-fly mode (Not Implemented Yet)   
std::string g_server_ip 		= "127.0.0.1";  // Server IP
std::string g_site_path 		= "/var/dashserver/www"; // Path of website files
std::string g_dash_path 		= "/tmp/dashserver";     // Path where the fragments, initiators and mpd are.
std::string g_mpd_name			= "dash.mpd";   // MPD file name
std::string g_controller_ip 	= "127.0.0.1";  // Controller server IP
std::string g_controller_url_path = "ArthronRest/api/dash_sessions";	// HTTP URL Path of controller server identify the register request

/******************************************************************************************
 * \brief 	This function connect and register the proxy on the
 * 			controller server, sending the ip and port used by the server
 * 			to receive the commands of the controller.
 ******************************************************************************************/
void registerOnController() {
	t_byte * buffer;
	Http * protocol = new Http();
	Socket * socket = new Socket();
	
	while(socket->Connect(g_controller_ip, g_controller_port) < 0) {
		std::this_thread::sleep_for (std::chrono::milliseconds(1000));
	}
	
	PRINT("[INFO] Connected to the controller.");
	
	g_server = new Socket(g_server_port);
	
	while(g_server->Bind() < 0 || g_server->Listen(10) < 0) {
		g_server->setPort(++g_server_port);
		PRINT("[INFO] Server port changed to " << g_server_port);
	}
	
	std::string json = "{\"session_id\" : \"init\", \"ip\" : \"" + g_server_ip + "\", \"port\" : \"" + std::to_string(g_server_port) + "\"}";
	
	protocol->createRequestHeader(g_controller_url_path, (t_size) json.length(), Http::int2ContentType(Http::ContentType::JSON), g_controller_ip + ":" + std::to_string(g_controller_port), Http::Method::POST);
	
	protocol->createBinaryPacket((t_byte*) json.c_str(), json.length());
	
	EXIT_IF(socket->Send(protocol->getBinaryPacket(), protocol->getBinarySize()) < 0, "");

	PRINT("[INFO] Sending register request...");
	
	buffer = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
	memset(buffer, 0, Http::BufferSize::MAX);

	PRINT("[INFO] Waiting for response...");
	
	EXIT_IF(socket->Receive(buffer, Http::BufferSize::MAX) < 0, "");
	
	PRINT("[INFO] Processing response...");

	protocol->processResponse(buffer);

	free(buffer);
	socket->Close();

	EXIT_IF(protocol->getReplyStatus() != Http::Status::CREATED, "[ERROR] The controller server responded with status code " << protocol->getReplyStatus());
}

/******************************************************************************************
 * \brief 	This function find certain session in the sessions list.
 *
 * \param 	list 	List of sessions.
 * \param 	id		ID of the requested session.
 *
 * \return	Return the index position of the requested session, if the
 * 			session not found, -1 is returned.
 ******************************************************************************************/
static int findSession(std::vector<Session*>& list, std::string id) {
	int i;
	
	for(i = 0; i < list.size(); i++) {
		if(!id.compare(list.at(i)->getID()))
			break;
	}
	
	i = i < list.size() ? i : -1;
	
	return i;
}

/******************************************************************************************
 * \brief 	This function keeps the server connected and processes the commands
 * 			received from the controller, which is to start or stop certain sessions.
 * 			When the command to start a new session is received, the server responds
 * 			to the controller with a JSON containing the IP of the server,
 * 			the TCP port used for HTTP connections, the UDP port used for receiving
 * 			arthron video stream and with the MPD file name to request and control
 * 			the Dash stream.
 ******************************************************************************************/
void startServer() {
	int buffsize, head_size, json_size, packet_size;
	std::vector<Session*> sessions;
	Http * protocol = new Http();
	std::string header, id, json;
	bool isjson;
	t_byte * buffer = (t_byte*) malloc(sizeof(t_byte) * Http::BufferSize::MAX);
	t_socket client;

	PRINT("[INFO] Server started! Running on port: " << g_server->getPort());
	
	while(true) {
		client = g_server->Accept();

		Socket::readFrom(client, buffer, Http::BufferSize::MAX, 30);
		
		protocol->processRequest(buffer);
		
		if(protocol->getContentType() == Http::ContentType::JSON) {
			std::string id, json(buffer);
			bool accept = true;
			
			try {
				json = json.substr(json.find("\r\n\r\n") + 4,  json.length() - 1);
			
				id = getJSONValue("session_id", json);

				if(id.length() > 0) {
					json = getJSONValue("command", json);
					
					if(json.length() > 0) {
						if(!json.compare("open")) {
							PRINT("[INFO] Command received: Start Session id:" << id);
							
							if(findSession(sessions, id) < 0) {
								sessions.push_back(new Session(id, g_controller_ip, g_initial_udp_port++, g_initial_tcp_port++, g_site_path, g_dash_profile, g_dash_path + "/" + id, g_mpd_name, g_dash_on_the_fly, g_on_the_fly_buffer_size));
								
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
						} else if(json.find("close") != std::string::npos) {
							PRINT("[INFO] Command received: Close Session id:" << id);
								
							int i = findSession(sessions, id);
								
							if(i >= 0) {
								sessions.at(i)->stop();
								//sessions.at(i)->~Session();
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
                PRINT("\n[ERROR] Invalid JSON received.");
				accept = false;
            }
			
			if(accept) {
				protocol->createResponseHeader(json.length(), Http::int2ContentType(Http::ContentType::JSON), Http::Status::OK);
				protocol->createBinaryPacket((t_byte*) json.c_str(), json.length());
			} else {
				protocol->createResponseHeader(0, "", Http::Status::BAD_REQUEST);
				protocol->createBinaryPacket(NULL, 0);
			}
		} else {
            PRINT("\n[ERROR] JSON not found.");
            protocol->createResponseHeader(0, "", Http::Status::BAD_REQUEST);
			protocol->createBinaryPacket(NULL, 0);
        }
			
		Socket::sendTo(client, protocol->getBinaryPacket(), protocol->getBinarySize());

		Socket::Close(client);

		memset(buffer, 0, Http::BufferSize::MAX);
	}

	free(buffer);
}


/******************************************************************************************
 * \brief 	Main Function
 *
 * \param 	argc 	amount of arguments of entry
 * \param 	argv 	list of arguments of entry
 *
 * \return 	Return 0 if successful and 1 if not.
 ******************************************************************************************/
int main(int argc, char *argv[]) {
	std::string::size_type sz;
	// add -on-the-fly e -buffer-size
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
				g_dash_profile = DashServer::Profile::LIVE;
				g_dash_path = "";
		}else if(!arg.compare("-on-demand")) {
			g_dash_profile = DashServer::Profile::ON_DEMAND;
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
                    "\n\r  Dash Profile: "              << (g_dash_profile == DashServer::Profile::LIVE ? "live" : g_dash_profile == DashServer::Profile::ON_DEMAND ? "on-demand" : "Invalid") << \
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
        "\nStarting with settings:\n"           << \
            "\n\r  Server IP: \""               << g_server_ip          << "\""\
            "\n\r  Server Port: "               << g_server_port        << \
            "\n\r  Initial HTTP Session Port: " << g_initial_tcp_port   << \
            "\n\r  Initial UDP Session Port: "  << g_initial_udp_port   << \
            "\n\r  Site Directory: \""          << g_site_path          << "\"" << \
            "\n\r  Dash Profile: "              << (g_dash_profile == DashServer::Profile::LIVE ? "live" : g_dash_profile == DashServer::Profile::ON_DEMAND ? "on-demand" : "Invalid") << \
            "\n\r  MPD Name: "                  << g_mpd_name 			<< \
            "\n\r  Dash Directory: \""          << g_dash_path          << "\"" << \
            "\n\r  Controller Server IP: \""    << g_controller_ip      << "\""\
            "\n\r  Controller Server Port: "    << g_controller_port    << \
            "\n\r  Controller URL Path: \""     << g_controller_url_path << "\"\n"\
    );
	
	registerOnController();
	startServer();
	
    return 0;
}
