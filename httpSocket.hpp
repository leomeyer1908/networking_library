#define BACK_LOG 5
#ifndef http_socket_hpp
#define http_socket_hpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class HTTP_Socket {
    private:
        struct sockaddr_in address;
        int httpSocket;
    public:
        HTTP_Socket(int port);
        int bindSocket();
        int listenSocket();
};

#endif