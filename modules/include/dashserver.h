/**
 * \file 	dashserver.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#ifndef DASH_H
#define DASH_H

#include <string>
#include "buffer.h"
#include "util.h"

/**
 * \brief This class manages and controls the dash processor.
 * 
 * \headerfile dashserver.h
 */

class DashServer {
    private:
    /**************************************************************************************/

        int port;                   // UDP port for receive video stream
        int profile;                // Dash profile used in this session
        std::string ip;             // IP of remote video stream origin
        std::string path;           // Dash files path
        std::string mpd;            // MPD file name
        bool isonthefly;            // On-the-fly mode on/off flag
        Buffer ** video_buffer;      // Buffer for Dash video fragments used in on-the-fly mode
        Buffer ** audio_buffer;      // Buffer for Dash audio fragments used in on-the-fly mode
        
        // Mirror
        DatagramSocket * rcv;
        DatagramSocket * snd;
        
        int engine_port;
        bool alive;
        
    
    /**************************************************************************************/
    public:
        /**
         * \enum    Profile
         * 
         * \brief   Constants of the types of supported profile.
         */
        enum Profile {
            LIVE = 0,       /*!< The stream origin is an Live stream of a camera. */
            ON_DEMAND = 1   /*!< The stream origin is an file. */
        };
        
        /**
         * \brief   Constructor
         *
         * \param   ip          IP of UDP stream origin server
         * \param   port        UDP port for receive video stream
         * \param   path        Dash files path
         * \param   mpd         MPD file name
         * \param   profile     Dash profile used in this session
         * \param   onthefly    On-the-fly mode on/off flag
         * \param   vid_buffer  Buffer for Dash video fragments
         * \param   aud_buffer  Buffer for Dash audio fragments
         */
        DashServer(std::string ip, int port, std::string path, std::string mpd, int profile, bool onthefly, Buffer **vid_buffer, Buffer **aud_buffer);

        /**
         * \brief   Destructor
         */
        virtual ~DashServer();

        /**
         * \brief   Bind the port
         *
         * \return  Return true if bind is sucessful, and false if not.
         */
        bool openConnection();

        /**
         * \brief   Start the Dash Server services.
         */
        void start();

        /**
         * \brief   Stop the Dash Server services.
         */
        void stop();

        /**
         * \brief   Set UDP port.
         *
         * \param   port    UDP port number.
         */
        void setPort(int port);

        /**
         * \brief   Get the used UDP port.
         *
         * \return  Return the used UDP port number.
         */
        int getPort();
};

#endif