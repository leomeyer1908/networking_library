#define BACK_LOG 5
#ifndef http_socket_hpp
#define http_socket_hpp
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

class Socket {
    private:
        struct addrinfo *address_info;
        int socket_fd;
    public:
        Socket(const char *port);
        char* getIP(char* url);
        int bindSocket();
        int listenSocket();
        int connectSocket(char *url, char *port);
};

#endif