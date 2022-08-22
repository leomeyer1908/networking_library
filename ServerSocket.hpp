#define BACK_LOG 20
#ifndef http_socket_hpp
#define http_socket_hpp
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>

class SeverSocket {
    private:
        struct addrinfo *address_info;
        int socket_fd;
    public:
        std::vector<int> clients = {};
        ServerSocket(const char *port);
        int bindSocket();
        int listenSocket();
        int acceptSocket();
};

#endif