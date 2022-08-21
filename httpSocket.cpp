#include "httpSocket.hpp"

HTTP_Socket::HTTP_Socket(const char *port) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, port, &hints, &address_info);
    if (status != 0) {
        throw std::runtime_error(gai_strerror(status));
    }

    socket_fd = socket(
        address_info->ai_family,
        address_info->ai_socktype,
        address_info->ai_protocol
    );
    if (socket_fd < 0)
        throw std::runtime_error("Socket not open"); 
}

int HTTP_Socket::bindSocket() {
    int socket_bind = bind(
        socket_fd,
        address_info->ai_addr,
        address_info->ai_addrlen
    );
    if (socket_bind < 0)
        throw std::runtime_error("Bind failed!");
    return socket_bind; 
}

int HTTP_Socket::listenSocket() {
    int socket_listen = listen(socket_fd, BACK_LOG);
    if (socket_listen < 0)
        throw std::runtime_error("Listen failed!");
    return socket_listen; 
}

char* HTTP_Socket::getIP(char* url) {
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(url, NULL, &hints, &result);
    if (status != 0) {
        throw std::runtime_error(gai_strerror(status));
    }
    //assume first result will be the good one
    struct sockaddr_in *ip = (struct sockaddr_in*) result->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->sin_addr.s_addr), ip_str, sizeof ip_str);
    return ip_str;
}