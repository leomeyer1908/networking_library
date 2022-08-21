#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

int main() {
    struct addrinfo hints, *res, *p;
    char ipstr[INET_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char website[] = "www.google.com";
    int status = getaddrinfo(website, NULL, &hints, &res);
    if (status != 0) {
        cout << "ERROR\n";
        return 2;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        struct sockaddr *socket_address_ptr = p->ai_addr;
        struct sockaddr_in *address = (struct sockaddr_in*) socket_address_ptr;
        inet_ntop(AF_INET, &(address->sin_addr.s_addr), ipstr, sizeof ipstr);
        
    }
    cout << sizeof ipstr << endl;
    cout << INET_ADDRSTRLEN << endl;


    return 0;
}