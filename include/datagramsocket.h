/******************************************************************************************
 * \file 	socket.h
 * 
 * \author 	Jorismar Barbosa Meira <jorismar.barbosa@lavid.ufpb.br>
 ******************************************************************************************/

#ifndef DATAGRAM_SOCKET_H
#define DATAGRAM_SOCKET_H

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"

/******************************************************************************************
 * \brief   This class implements the UDP sockets.
 * 
 * \headerfile  datagramsocket.h
 ******************************************************************************************/

class DatagramSocket {
    private:
    /**************************************************************************************/

        int svr_socket;
        int svr_port;
        std::string svr_address;

        struct sockaddr_in svr_addr;
        struct sockaddr_in to_addr;
        struct sockaddr_in from_addr;
        
        socklen_t to_addrlen;
        socklen_t from_addrlen;

    /**************************************************************************************/
    public:
        /**********************************************************************************
         * \brief   Constructor.
         *
         * \param   port    Port for bind.
         **********************************************************************************/
        DatagramSocket(int port);

        /**********************************************************************************
         * \brief   Constructor.
         *
         * \param   address     IP address of server.
         * \param   port        Port of server.
         **********************************************************************************/
        DatagramSocket(std::string address, int port);

        /**********************************************************************************
         * \brief   Destructor.
         **********************************************************************************/
        virtual ~DatagramSocket();

        /**********************************************************************************
         * \brief   Associate a socket with Local IP address and a defined port number.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int Bind();

        /**********************************************************************************
         * \brief   Receive data on a socket (Timeout is supported).
         *
         * \param   buffer  Storage buffer pointer.
         * \param   size    Storage buffer size.
         *
         * \return  Returns the number of bytes actually received, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int Receive(t_byte* buffer, t_size size);

        /**********************************************************************************
         * \brief   Send data to a remote server.
         *
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         * \param   address Address of remote server.
         * \param   port    Port of remote server.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int SendTo(t_byte* buffer, t_size size, std::string address, int port);

        /**********************************************************************************
         * \brief   Send data out over a registered socket.
         *
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int Send(t_byte* buffer, t_size size);

        /**********************************************************************************
         * \brief   Send the reply data to the client of the last mensage received.
         *
         * \param   buffer  Data buffer pointer.
         * \param   size    Data buffer size.
         *
         * \return  Returns the number of bytes actually sent, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int Reply(t_byte* buffer, t_size size);

        /**********************************************************************************
         * \brief   Close the main and remote socket descriptor.
         *
         * \return  Returns zero on success, or -1 on error (and errno will be set accordingly).
         **********************************************************************************/
        int Close();

        /**********************************************************************************
         * \brief   Get registered port.
         *
         * \return  Return registered port number.
         **********************************************************************************/
        int getPort();

         /*********************************************************************************
         * \brief   Get registered address.
         *
         * \return  Return registered address.
         **********************************************************************************/
        std::string getHostAddress();

         /*********************************************************************************
         * \brief   Register a client on the socket to use Send method.
         *
         * \param   address Address of remote server.
         * \param   port    Port of remote server.
         **********************************************************************************/
        void registerclient(std::string address, int port);

         /*********************************************************************************
         * \brief   Get the socket.
         *
         * \return  Return the main socket.
         **********************************************************************************/
        int getSocket();
};

#endif
