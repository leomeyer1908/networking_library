#define BUFFER_SIZE 8192
#ifndef http_server_hpp
#define http_server_hpp
#include "ServerSocket.hpp"
#include <poll.h>

class HTTP_Server {
    private:
        bool is_simple_request;
        char buf[BUFFER_SIZE];
        void evaluate_request(char* buf);
    public:
        HTTP_Server(const char *port, char* path, int path_size);
};

#endif