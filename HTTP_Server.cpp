#include "HTTP_Server.hpp"



//I PROBABLY DONT NEED STATUS_CLASS SINCE THIS WILL LIKELY ONLY BE USED
//FOR SERVER TO GENERATE REASON PHRASE, SO IT WILL ALWAYS KNOW THE CORRECT ONE
void get_reason_phrase(char* status_code, char* reason_phrase) {
    const int STATUS_CODES_NUM = 15;
    const int CLASSES_NUM = 4;
    struct status_code_reason_phrase {
        char status_code[3];
        char* reason_phrase;
    };

    struct status_code_class {
        char status_code_first_digit;
        char* reason_phrase;
    };

    struct status_code_reason_phrase reason_phrases[STATUS_CODES_NUM] = {
        {{'2', '0', '0'}, "OK"},
        {{'2', '0', '1'}, "Created"},
        {{'2', '0', '2'}, "Accepted"},
        {{'2', '0', '4'}, "No Content"},
        {{'3', '0', '1'}, "Moved Permanently"},
        {{'3', '0', '2'}, "Moved Temporarily"},
        {{'3', '0', '4'}, "Not Modified"},
        {{'4', '0', '0'}, "Bad Request"},
        {{'4', '0', '1'}, "Unauthorized"},
        {{'4', '0', '3'}, "Forbidden"},
        {{'4', '0', '4'}, "Not Found"},
        {{'5', '0', '0'}, "Internal Server Error"},
        {{'5', '0', '1'}, "Not Implemented"},
        {{'5', '0', '2'}, "Bad Gateway"},
        {{'5', '0', '3'}, "Service Unavailable"}
    };
    
    bool is_known_status_code = false;
    for (int i = 0; i < STATUS_CODES_NUM; i++) {
        if (status_code == reason_phrases[i].status_code) {
            reason_phrase = reason_phrases[i].reason_phrase;
            is_known_status_code = true;
            break;
        }
    }

    if (is_known_status_code) {
        return;
    }

    struct status_code_class classes[CLASSES_NUM] = {
        {'2', "OK"},
        {'4', "Bad Request"},
        {'5', "Internal Server Error"}
    };

    for (int i = 0; i < CLASSES_NUM; i++) {
        if (*status_code == classes[i].status_code_first_digit) {
            reason_phrase = classes[i].reason_phrase;
            break;
        }
    }
}


void decode_uri(char* uri, int uri_size, char* decoded) {
    const int ESCAPE_CODES_NUM = 23;
    struct escape_code {
        char hex_code[2];
        char character;
    };
    struct escape_code escape_codes[ESCAPE_CODES_NUM] = {
        {{'2', '0'}, ' '},
        {{'3', 'C'}, '<'},
        {{'3', 'E'}, '>'},
        {{'2', '3'}, '#'},
        {{'2', '5'}, '%'},
        {{'2', 'B'}, '+'},
        {{'7', 'B'}, '{'},
        {{'7', 'D'}, '}'},
        {{'7', 'C'}, '|'},
        {{'5', 'C'}, '\\'},
        {{'5', 'E'}, '^'},
        {{'7', 'E'}, '~'},
        {{'5', 'B'}, '['},
        {{'5', 'D'}, ']'},
        {{'6', '0'}, '`'},
        {{'3', 'B'}, ';'},
        {{'2', 'F'}, '/'},
        {{'3', 'F'}, '?'},
        {{'3', 'A'}, ':'},
        {{'4', '0'}, '@'},
        {{'3', 'D'}, '='},
        {{'2', '6'}, '&'},
        {{'2', '4'}, '$'}
    };
    int decoded_index = 0;
    for (int i = 0; i < uri_size; i++) {
        if (uri[i] == '%') {
            for (int j = 0; j < ESCAPE_CODES_NUM; j++) {
                if (escape_codes[j].hex_code[0] == uri[i+1]) {
                    if (escape_codes[j].hex_code[1] == uri[i+2]) {
                        decoded[decoded_index] = escape_codes[j].character;
                        i += 2;
                    } 
                }
            }
        }
        else {
            decoded[decoded_index] = uri[i];
        }
        decoded_index++;
    }
}

HTTP_Server::HTTP_Server(const char *port, char* path, int path_size) {
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
                    evaluate_request(buf);
                    

                    /*GENERATE RESPONSE*/
                    if (!is_simple_request) {
                        char status_line[] = "HTTP/1.0 200";
                    }
                    




                    //GENERAL HEADER
                    //RESPONSE HEADER
                    //ENTITY HEADER
                    //ENTITY BODY






                    //print request to console
                    for (char* ptr = buf; ptr < buf + sizeof(buf); ptr++) {
                        std::cout << *ptr;
                    }
                    std::cout << std::endl;
                    //char msg[] = "HTTP/1.0 200 OK\r\n\r\n<html><body><h1>Hello World!</h1></body></html>";
                    // server.sendData(poll_fds[i].fd, msg, sizeof(msg)/sizeof(msg[0]));
                    //server.sendData(poll_fds[i].fd, html, html_size);
                }
            }
        }
    }
}

void HTTP_Server::evaluate_request(char* buf) {
    const char GET[] = "GET";
    const char HEAD[] = "HEAD";
    const char POST[] = "POST";
    const char* methods[3] = {GET, HEAD, POST};
    int method = -1; //GET = 0, HEAD = 1, POST = 2; NONE = -1
    /*METHOD*/
    char* request_method = buf;
    int request_method_size = 0;
    for (char* ptr = buf; ptr < buf + sizeof(buf); ptr++) {
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
    int escape_codes_num = 0;
    for (char* ptr = request_uri; ptr < buf + sizeof(buf); ptr++) {
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
        if (*ptr == '%') {
            escape_codes_num++;
        }
        request_uri_size++;
    }
    //CHECK IF % WITH INVALID CODES CAN GIVE ERROR
    int decoded_uri_size = request_uri_size - escape_codes_num*2;
    if (decoded_uri_size == 0) {
        decoded_uri_size = 1;
    }
    char decoded_uri[decoded_uri_size];
    if (decoded_uri_size == 1) {
        decoded_uri[0] = '/';
    }
    decode_uri(request_uri, request_uri_size, decoded_uri);

    /*HTTP-VERSION*/
    char* HTTP_version = request_uri+request_uri_size+1;
    int HTTP_version_size = 0;
    for (char* ptr = HTTP_version; ptr < buf + sizeof(buf); ptr++) {
        if (*ptr == '\r') {
            if (*(ptr+1) == '\n') {
                //if it does not have a CRLF then do error
                break;
            }
            //GIVE ERROR
        }
        HTTP_version_size++;
    }

    //GENERAL HEADER
    //REQUEST HEADER
    //ENTITY HEADER
    //ENTITY BODY
}