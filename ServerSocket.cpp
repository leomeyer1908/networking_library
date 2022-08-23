#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const char *port) {
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

ServerSocket::~ServerSocket() {
    close(socket_fd);
    for (int i = 0; i < client_sockets.size(); i++) {
        close(client_sockets[i]);
    }
}

int ServerSocket::bindSocket() {
    //makes socket be able to bind to port if it had binded to it before
    int yes = 1;
    int status = setsockopt(
        socket_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &yes,
        sizeof(int)
    );
    if (status == -1) {
        perror("setsockopt");
        exit(1); 
    }
    int socket_bind = bind(
        socket_fd,
        address_info->ai_addr,
        address_info->ai_addrlen
    );
    if (socket_bind < 0)
        throw std::runtime_error("Bind failed!");
    return socket_bind; 
}

int ServerSocket::listenSocket() {
    int socket_listen = listen(socket_fd, BACK_LOG);
    if (socket_listen < 0)
        throw std::runtime_error("Listen failed!");
    return socket_listen; 
}

int ServerSocket::acceptSocket() {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;
    int client_socket_fd = accept(
        socket_fd,
        (struct sockaddr *) &client_addr,
        &client_addr_size
    );
    if (client_socket_fd != -1)
        client_sockets.push_back(client_socket_fd);
    return client_socket_fd;
}

int ServerSocket::sendData(int client_fd, char *data, int data_size) {
    int bytes_sent;
    //make a while loop to make sure all the data is sent
    bytes_sent = send(client_fd, data, data_size, 0);
}

int ServerSocket::recvData(int client_fd, char *buffer, int buffer_size) {
    int status = recv(client_fd, buffer, buffer_size, 0);
    if (status == -1)
        throw std::runtime_error("Receive message failed!");
    else if (status == 0) {
        close(client_fd);
        for (int i = 0; i < client_sockets.size(); i++) {
            if (client_sockets.at(i) == client_fd)
                client_sockets.erase(client_sockets.begin()+i);
        }
    }
    return status;
}

// int Socket::connectSocket(char *url, char* port) {
//     struct addrinfo hints, *result;

//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;

//     int status = getaddrinfo(url, port, &hints, &result);
//     if (status != 0) {
//         throw std::runtime_error(gai_strerror(status));
//     }

//     int socket_connect = connect(
//         socket_fd,
//         result->ai_addr,
//         result->ai_addrlen
//     );

//     if (socket_connect < 0)
//         throw std::runtime_error("Connect failed!");

//     return socket_connect;
// }

// char* Socket::getIP(char* url) {
//     struct addrinfo hints, *result;

//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;

//     int status = getaddrinfo(url, NULL, &hints, &result);
//     if (status != 0) {
//         throw std::runtime_error(gai_strerror(status));
//     }
//     //assume first result will be the good one
//     struct sockaddr_in *ip = (struct sockaddr_in*) result->ai_addr;
//     char ip_str[INET_ADDRSTRLEN];
//     inet_ntop(AF_INET, &(ip->sin_addr.s_addr), ip_str, sizeof ip_str);
//     return ip_str;
// }