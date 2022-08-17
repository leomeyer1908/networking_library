#include "httpSocket.hpp"

HTTP_Socket::HTTP_Socket(int port) {
    httpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (httpSocket < 0)
        throw std::runtime_error("Socket not open"); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(port); 
}

int HTTP_Socket::bindSocket() {
    int socket_bind = bind(
        httpSocket,
        (struct sockaddr *)&address,
        sizeof(address)
    );
    if (socket_bind < 0)
        throw std::runtime_error("Bind failed!");
    return socket_bind; 
}

int HTTP_Socket::listenSocket() {
    int socket_listen = listen(httpSocket, BACK_LOG);
    if (socket_listen < 0)
        throw std::runtime_error("Listen failed!");
    return socket_listen; 
}