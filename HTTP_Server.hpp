#define BUFFER_SIZE 8192
#ifndef http_server_hpp
#define http_server_hpp
#include "ServerSocket.hpp"
#include <poll.h>
#include <fstream>
#include <string>

class HTTP_Server {
    private:
        bool is_simple_request;
        char buf[BUFFER_SIZE];
    public:
        struct request_line {
            char* method;
            char* request_uri;
            char* http_version;
            int method_size;
            int request_uri_size;
            int http_version_size;
        };
        struct full_request {
            struct request_line request_line;
            char* general_header;
            char* request_header;
            char* entity_header;
            char* entity_body;
        };
        void evaluate_request(char* buf, struct full_request* request);
        HTTP_Server(const char *port, char* path, int path_size);
};

#endif