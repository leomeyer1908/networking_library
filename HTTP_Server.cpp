#include "HTTP_Server.hpp"

HTTP_Server::HTTP_Server(const char *port, char* html, int html_size) {
    int poll_count;
    std::vector<pollfd> poll_fds;
    ServerSocket server("8080");
    server.bindSocket();
    server.listenSocket();
    poll_fds.push_back(pollfd());
    poll_fds[0].fd = server.socket_fd;
    poll_fds[0].events = POLLIN;
    while (1) {
        poll_count = poll(&poll_fds[0], poll_fds.size(), -1);
        if (poll_count == -1) {
            throw std::runtime_error("Error with poll"); 
        }
        for (int i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents && POLLIN) {
                if (i == 0) {
                    poll_fds.push_back(pollfd());
                    poll_fds[poll_fds.size()-1].fd = server.acceptSocket();
                    poll_fds[poll_fds.size()-1].events = POLLIN;
                }
                else {
                    std::cout << poll_fds[i].fd;
                    memset(buf, 0, sizeof buf);
                    int bytes_recv = server.recvData(poll_fds[i].fd, buf, sizeof buf);
                    if (bytes_recv == 0) {
                        poll_fds.erase(poll_fds.begin()+i);
                        i--;
                        continue;
                    }

                    /*EVALUATE REQUESTS*/
                    const char GET[] = "GET";
                    const char HEAD[] = "HEAD";
                    const char POST[] = "POST";
                    const char* methods[3] = {GET, HEAD, POST};
                    int method = -1; //GET = 0, HEAD = 1, POST = 2; NONE = -1
                    bool is_simple_request;
                    /*METHOD*/
                    char* request_method = buf;
                    int request_method_size = 0;
                    for (char* ptr; ptr < buf + sizeof(buf); ptr++) {
                        if (*ptr == ' ') {
                            break;
                        }
                        request_method_size++;
                    }
                    bool is_same_str;
                    for (int i = 0; i < 3; i++) {
                        is_same_str = true;
                        for (int j = 0; j < request_method_size; j++) {
                            if (methods[i][j] != request_method[j]) {
                                is_same_str = false;
                                break;
                            }
                        }
                        if (is_same_str) {
                            method = i;
                            break;
                        }
                    }
                    if (method == -1) {
                        //SEND OUT STATUS CODE 501
                    }
                    /*REQUEST-URI*/
                    char* request_uri = (buf+request_method_size+1);
                    int request_uri_size = 0;
                    for (char* ptr; ptr < buf + sizeof(buf); ptr++) {
                        if (*ptr == ' ') {
                            break;
                        }
                        if (*ptr == '\r') {
                            if (*(ptr+1) == '\n') {
                                if (method == 0) {
                                    is_simple_request = true;
                                    break;
                                }
                                //INVALID REQUEST, DO SOMETHING HERE
                                break;
                            }
                            //INVALID REQUEST, DO SOMETHING HERE
                        }
                        request_uri_size++;
                    }
                    /*HTTP-VERSION*/







                    //print request to console
                    for (char* ptr; ptr < buf + sizeof(buf); ptr++) {
                        std::cout << *ptr;
                    }
                    std::cout << std::endl;
                    // char msg[] = "HTTP/1.0 200 OK\r\n\r\n<html><body><h1>Hello World!</h1></body></html>";
                    // server.sendData(poll_fds[i].fd, msg, sizeof(msg)/sizeof(msg[0]));
                    server.sendData(poll_fds[i].fd, html, html_size);
                }
            }
        }
    }
}