#define BACK_LOG 20
#ifndef server_socket_hpp
#define server_socket_hpp
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>

class ServerSocket {
    private:
        struct addrinfo *address_info;
        int socket_fd;
    public:
        std::vector<int> client_sockets = {};
        ServerSocket(const char *port);
        ~ServerSocket();
        int bindSocket();
        int listenSocket();
        int acceptSocket();
        int sendData(int client_fd, char *data, int data_size);
        int recvData(int client_fd, char *buffer, int buffer_size);
};

#endif