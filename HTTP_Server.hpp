#define BUFFER_SIZE 8192
#ifndef http_server_hpp
#define http_server_hpp
#include "ServerSocket.hpp"
#include <poll.h>

class HTTP_Server {
    private:
        char buf[BUFFER_SIZE];
    public:
        HTTP_Server(const char *port, char* html, int html_size);
};

#endif