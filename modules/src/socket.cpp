#include "socket.h"

/******************************************************************************************/

Socket::Socket() {
    this->main_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(main_socket < 0)
        perror("ERROR on open socket");
    else {
        this->port      = 0;
        this->timeout   = 0;
        this->remote_socket_len = 0;
        this->connected = false;
        this->bound     = false;
        this->listening = false;
        this->closed    = false;
    }
}

/******************************************************************************************/

Socket::Socket(int port) {
    this->main_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(main_socket < 0)
        perror("ERROR on open socket");
    else {
        this->port      = port;
        this->timeout   = 0;
        this->remote_socket_len = 0;
        this->connected = false;
        this->bound     = false;
        this->listening = false;
        this->closed    = false;
    }
}

/******************************************************************************************/

Socket::~Socket() {
    this->Close();
}

/******************************************************************************************/

int Socket::Bind() {
    if(port == 0) {
        PRINT("ERROR on binding: Port not defined.");
        return -1;
    }
    
    return this->Bind(this->port);
}

/******************************************************************************************/

int Socket::Bind(int port) {
    int r;
    this->port = port;
    
    if(port < 1) {
        PRINT("ERROR on binding: Invalid port.");
        return -1;
    }
    
    memset((char*) &this->main_addr, 0, sizeof(this->main_addr));
    
    this->main_addr.sin_family = AF_INET;
    this->main_addr.sin_addr.s_addr = INADDR_ANY;
    this->main_addr.sin_port = htons(this->port);
    
    r = bind(this->main_socket, (struct sockaddr*) &this->main_addr, sizeof(this->main_addr));
    
    if(r < 0)
        perror("ERROR on binding");
    else this->bound = true;
    
    return r;
}

/******************************************************************************************/

int Socket::Listen(int backlog) {
    int r;
    
    r = listen(this->main_socket, backlog);
    
    if(r < 0)
        perror("ERROR on listening");
    else this->listening = true;
    
    return r;
}

/******************************************************************************************/

t_socket Socket::Accept() {
    this->remote_socket = accept(this->main_socket, (struct sockaddr*) &this->remote_socket, &this->remote_socket_len);
    
    if(this->remote_socket < 0)
        perror("ERROR on accept");
    
    return remote_socket;
}

/******************************************************************************************/

int Socket::Connect(std::string ip, int port) {
    int r;
    
    this->main_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    this->main_addr.sin_family = AF_INET;
    this->main_addr.sin_port = htons(port);
    
    r = connect(this->main_socket, (struct sockaddr*) &this->main_addr, sizeof(this->main_addr));
    
    if(r < 0)
        perror("ERROR on connect");
    else this->connected = true;
    
    return r;
}

/******************************************************************************************/

int Socket::Read(t_byte * buffer, t_size size) {
    int r;
    
    r = read(this->main_socket, buffer, size);

    if(r < 0)
        perror("ERROR on read");

    return r;
}

/******************************************************************************************/

int Socket::Send(t_byte * buffer, t_size size) {
    int r;
    
    signal(SIGPIPE, SIG_IGN);
    
    r = send(this->main_socket, buffer, size, 0);
    
    if(r < 0)
        perror("ERROR on send");

    return r;
}

/******************************************************************************************/

int Socket::Reply(t_byte * buffer, t_size size) {
    int r;
    
    signal(SIGPIPE, SIG_IGN);
    
    r = send(this->remote_socket, buffer, size, 0);
    
    if(r < 0)
        perror("ERROR on reply");

    return r;
}

/******************************************************************************************/

int Socket::Receive(t_byte * buffer, t_size size) {
    int r;
    
    r = recv(this->main_socket, buffer, size, 0);
    
    if(r < 0)
        perror("ERROR on receive");

    return r;
}

/******************************************************************************************/

int Socket::Close() {
    int r;
    
    r = close(this->main_socket);

    if(r >= 0 && this->remote_socket_len > 0) {
        r = close(this->remote_socket);
    }

    if(r < 0) {
        perror("ERROR on close");
    } else this->closed = true;

    return r;
}

/******************************************************************************************/

int Socket::CloseServer() {
    int r;
    
    r = close(this->main_socket);

    if(r < 0)
        perror("ERROR on close");
    else this->closed = true;
    
    return r;
}

/******************************************************************************************/

int Socket::CloseClient() {
    int r;
    
    r = close(this->remote_socket);

    if(r < 0)
        perror("ERROR on close");
    else this->closed = true;

    return r;
}

/******************************************************************************************/

int Socket::getPort() {
    return this->port;
}

/******************************************************************************************/

void Socket::setPort(int port) {
    this->port = port;
}

/******************************************************************************************/

void Socket::setTimeout(int sec) {
    struct timeval tv;
    
    tv.tv_sec  = sec;
    tv.tv_usec = 0;
    
    setsockopt(this->main_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    this->timeout = sec;
}

/******************************************************************************************/

bool Socket::isConnected() {
    return this->connected;
}

/******************************************************************************************/

bool Socket::isBound() {
    return this->bound;
}

/******************************************************************************************/

bool Socket::isListening() {
    return this->listening;
}

/******************************************************************************************/

bool Socket::isClosed() {
    return this->closed;
}
