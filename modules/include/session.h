/**
 * \file 	session.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <cstdlib>
#include "webserver.h"
#include "buffer.h"
#include "util.h"
#include "dashserver.h"

/**
 * \brief This class manages and controls the session running.
 * 
 * \headerfile session.h
 */

class Session {
    private:
    /******************************************************************************************/

        std::string id;             // Session ID
        Buffer * video_dash_buffer; // Buffer for Dash video fragments used in on-the-fly mode
        Buffer * audio_dash_buffer; // Buffer for Dash audio fragments used in on-the-fly mode
        Webserver * webserver;      // Webserver object
        DashServer * dashserver;    // DashServer object

    /******************************************************************************************/

    public:
        /**
         * \brief   Constructor
         *
         * \param   id            Session ID
         * \param   ip            IP of UDP stream origin server
         * \param   udp_port      UDP port for receive video stream
         * \param   http_port     TCP port for HTTP users access
         * \param   site_path     Website files path
         * \param   dash_profile  Dash profile used in this session
         * \param   dash_path     Dash files path
         * \param   mpd           MPD file name
         * \param   is_on_the_fly On-the-fly mode on/off flag
         * \param   buffer_size   Size of buffer Dash fragments used in on-the-fly mode
         */
        Session(std::string id, std::string ip, int udp_port, int http_port, std::string site_path, int dash_profile, std::string dash_path, std::string mpd, bool is_on_the_fly, t_size buffer_size);

        /**
         * \brief   Destructor
         */
        virtual ~Session();
        
        /**
         * \brief   This function bind the UDP port
         *
         * \return  Return true if bind is sucessfuly and false if not.
         */
        bool bindUdpPort();

        /**
         * \brief   This function bind the UDP port
         *
         * \return  Return true if bind is sucessfuly and false if not.
         */
        bool bindHttpPort();
        
        /**
         * \brief   This function start the Webserver and Dash Processor
         */
        void start();

        /**
         * \brief   This function stop the Webserver and Dash Processor
         */
        void stop();
        
        /**
         * \brief   Get ID session
         *
         * \return  Return the ID of this session.
         */
        std::string getID();
        
        /**
         * \brief   Get UDP Port
         *
         * \return  Return the UDP port used to receive video stream.
         */
        int getUdpPort();

        /**
         * \brief   Get TCP/HTTP port
         *
         * \return  Return the TCP port used to send dash video stream
         */
        int getHttpPort();

        /**
         * \brief   This function set the UDP port
         */
        void setUdpPort(int port);

        /**
         * \brief   This function set the TCP/HTTP port
         */
        void setHttpPort(int port);
};

#endif