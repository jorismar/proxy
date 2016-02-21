/**
 * \file 	webserver.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <cstdio>
#include <cstddef>

#include "buffer.h"
#include "socket.h"
#include "util.h"
#include "virtualfile.h"
#include "http.h"
#include "dashserver.h"

/**
 * \brief This class manages and controls the server for web (http) clients.
 * 
 * \headerfile webserver.h
 */

class Webserver {
    private:
    /******************************************************************************************/

        Http * header;              // HTTP Protocol Object
        Socket * socket;            // TCP Socket for server
        Buffer ** v_dash_buffer;    // Dash video buffer for on-the-fly mode
        Buffer ** a_dash_buffer;    // Dash audio buffer for on-the-fly mode
        std::string page_path;      // Path of the Website files
        std::string dash_path;      // Path of the Dash files
        int port;                   // TCP port used for the server
        bool alive;                 // Flag to keep the server alive.
        bool on_the_fly;            // On-the-fly mode flag

    /******************************************************************************************/
        
    public:
        
        /**
         * \brief   Constructor
         * 
         * \param   svr_port        TCP port for HTTP server
         * \param   is_on_the_fly   Dash on-the-fly mode flag
         * \param   video_buffer    Dash video buffer for on-the-fly mode.
         * \param   audio_buffer    Dash audio buffer for on-the-fly mode.
         * \param   path            Website files path
         * \param   dash_path       Dash files path
         */
        Webserver(int svr_port, bool is_on_the_fly, Buffer **video_buffer, Buffer **audio_buffer, std::string path, std::string dash_path);

        /**
         * \brief   Destructor
         */
        virtual ~Webserver();

        /**
         * \brief   This method start and bind TCP sockets for HTTP server
         *
         * \return  Return true if TCP socket open and bind sucessfuly and false if not.
         */
        bool openConnection();

        /**
         * \brief   This method start the HTTP server
         */
        void start();

        /**
         * \brief   This method stop the HTTP server
         */
        void stop();

        /**
         * \brief   This method process file request and return then.
         *
         * \param   filename    Name of requested file.
         *
         * \return  Return a virtual objetc of the requested file, or NULL if a error occurs.
         */
        VirtualFile* getFile(std::string filename);

        /**
         * \brief   This method read a file on the computer disc.
         *
         * \param   path        Requested file location.
         * \param   filetype    Type of requested file.
         *
         * \return  Return a virtual objetc of the requested file, or NULL if a error occurs.
         */
        VirtualFile* readFile(std::string path, std::string filetype);

        /**
         * \brief   This method read a file on the buffer.
         *
         * \param   filename    Name of requested file.
         *
         * \return  Return a virtual objetc of the requested file, or NULL if a error occurs.
         */
        VirtualFile* readExternalBuffer(std::string filename);

        /**
         * \brief   This method start a thread to meet a specific client.
         *
         * \param   cl  Socket of a connected client.
         */
        void startClient(t_socket cl);

        /**
         * \brief   Set the TCP port
         *
         * \param   port    TCP port for the HTTP connections
         */
        void setPort(int port);

        /**
         * \brief   Get the TCP port
         *
         * \return   Return the TCP port for the HTTP connections
         */
        int getPort();
};

#endif
