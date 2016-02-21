/**
 * \file 	socket.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"

/**
 * \brief This class implements the TCP sockets.
 * 
 * \headerfile socket.h
 */

class Socket {
    private:
    /******************************************************************************************/

        t_socket main_socket;           // Main socket
        t_socket remote_socket;         // Remote connected socket
        socklen_t remote_socket_len;    // Remote connected socket length
        struct sockaddr_in main_addr;   // Structure for handling internet addresses
        int port;                       // TCP Port used for bind
        int timeout;                    // Timeout
        bool connected;                 // Connection state flag
        bool bound;                     // Bind state flag
        bool listening;                 // Listen state flag
        bool closed;                    // Open socket state flag

    /******************************************************************************************/
    public:
        /**
         * \brief   Constructor.
         */
        Socket();

        /**
         * \brief   Constructor.
         *
         * \param   port    Port for bind.
         */
        Socket(int port);

        /**
         * \brief   Destructor.
         */
        virtual ~Socket();

        /**
         * \brief   Associate a socket with Local IP address and a defined port number.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Bind();

        /**
         * \brief   Associate a socket with Local IP address and a port number.
         *
         * \param   port    Port number.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Bind(int port);

        /**
         * \brief   Associate a socket with an IP address and a port number (NOT IMPLEMENTED YET).
         *
         * \param   ip      IP address.
         * \param   port    Port number.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Bind(std::string ip, int port);

        /**
         * \brief   Tell a socket to listen for incoming connections.
         *
         * \param   backlog      Length of pending connections queue.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Listen(int backlog);

        /**
         * \brief   Accept an incoming connection on a listening socket.
         *
         * \return  Returns the newly connected socket descriptor, or -1 on error (and errno will be set accordingly).
         */
        t_socket Accept();

        /**
         * \brief   Connect a socket to a server.
         *
         * \param   ip      Remote IP address.
         * \param   port    Remote Port number.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Connect(std::string ip, int port);

        /**
         * \brief   Read data on a socket.
         *
         * \param   buffer  Storage buffer pointer.
         * \param   size    Storage buffer size.
         *
         * \return  Returns the number of bytes actually readed, or -1 on error (and errno will be set accordingly).
         */
        int Read(t_byte* buffer, t_size size);

        /**
         * \brief   Receive data on a socket (Timeout is supported).
         *
         * \param   buffer  Storage buffer pointer.
         * \param   size    Storage buffer size.
         *
         * \return  Returns the number of bytes actually received, or -1 on error (and errno will be set accordingly).
         */
        int Receive(t_byte* buffer, t_size size);

        /**
         * \brief   Send data out over a socket.
         *
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         */
        int Send(t_byte* buffer, t_size size);

        /**
         * \brief   Send data out over a remote connected socket.
         *
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         */
        int Reply(t_byte* buffer, t_size size);

        /**
         * \brief   Close the main and remote socket descriptor.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int Close();

        /**
         * \brief   Close the main socket descriptor.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int CloseServer();

        /**
         * \brief   Close the remote socket descriptor.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        int CloseClient();

        /**
         * \brief   Set main socket port.
         *
         * \param   port    Port number.
         */
        void setPort(int port);

        /**
         * \brief   Set timeout.
         *
         * \param   sec     Timeout value in seconds.
         */
        void setTimeout(int sec);

        /**
         * \brief   Get main socket port.
         *
         * \return  Return port number of socket.
         */
        int getPort();

        /**
         * \brief   Get timeout value.
         *
         * \return  Return configured timeout value.
         */
        int getTimeout();

        /**
         * \brief   Check if socket is connected.
         *
         * \return  Return true if socket is connected and false if not.
         */
        bool isConnected();

        /**
         * \brief   Check if socket is bound.
         *
         * \return  Return true if socket is bound and false if not.
         */
        bool isBound();

        /**
         * \brief   Check if socket is listening.
         *
         * \return  Return true if socket is listening and false if not.
         */
        bool isListening();

        /**
         * \brief   Check if socket is closed.
         *
         * \return  Return true if socket is closed and false if not.
         */
        bool isClosed();

        /**
         * \brief   Receive data on a socket (Timeout is supported).
         *
         * \param   socket  Local and connected socket descriptor.
         * \param   buffer  Storage buffer pointer.
         * \param   size    Storage buffer size.
         * \param   timeout Timeout value in seconds.
         *
         * \return  Returns the number of bytes actually received, or -1 on error (and errno will be set accordingly).
         */
        static int readFrom(t_socket socket, t_byte * buffer, t_size size, int timeout) {
            int r;
            struct timeval tv;
            
            if(timeout > 0) {
                tv.tv_sec  = timeout;
                tv.tv_usec = 0;
            
                setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            }
            
            r = recv(socket, buffer, size, 0);
            
            if(r < 0)
                perror("ERROR on receive");
        
            return r;
        }
        
        /**
         * \brief   Read data on a socket.
         *
         * \param   socket  Local and connected socket descriptor.
         * \param   buffer  Storage buffer pointer.
         * \param   size    Storage buffer size.
         *
         * \return  Returns the number of bytes actually readed, or -1 on error (and errno will be set accordingly).
         */
        static int readFrom(t_socket socket, t_byte * buffer, t_size size) {
            int r;
        
            r = read(socket, buffer, size);
        
            if(r < 0)
                perror("ERROR on receive");

            return r;
        }
        
        /**
         * \brief   Send data out over a socket.
         *
         * \param   socket  Remote and connected socket descriptor.
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         */
        static int sendTo(t_socket socket, t_byte * buffer, t_size size) {
            int r;
        
            signal(SIGPIPE, SIG_IGN);
        
            r = send(socket, buffer, size, 0);
        
            if(r < 0)
                perror("ERROR on send");

            return r;
        }
        
        /**
         * \brief   Close a socket descriptor
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         */
        static int Close(t_socket socket) {
            int r;
            
            r = close(socket);
        
            if(r < 0)
                perror("ERROR on close");

            return r;
        }
};

#endif