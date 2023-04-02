#include "tcpclient.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: minhttp <url>\n");
        return -1;
    }

    struct parsed_url url;
    parse_url(&url, argv[1]);
    

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *addresses = NULL;
    int rv = getaddrinfo(url.hostname, url.port, &hints, &addresses);
    if(rv < 0){
        printf("getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    int sockfd = try_connection(addresses);
    if(sockfd < 0){
        printf("Hostname can't be reached. Connection failed\n");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(addresses);

    char send_msg_buf[4096] = {0};
    char recv_msg_buf[4096 + 32 + 16 + 5] = {0};

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    FD_SET(0, &readfds);        //for stdin

    //while(!isQuitMessage(send_msg_buf)){
    while(1){
        fd_set cpy_fds = readfds;

        if(select(sockfd+1, &cpy_fds, 0, 0, &timeout) < 0){
            perror("select");
        }

        if(FD_ISSET(sockfd, &cpy_fds)){ //receive message 
            int recv_bytes = recv(sockfd, recv_msg_buf, sizeof(recv_msg_buf), 0);
            recv_msg_buf[recv_bytes] = '\0';
            if(recv_bytes < 1){
                printf("Server connection closed");
                FD_CLR(sockfd, &readfds);
                break;
            }else{
                printf("> %s", recv_msg_buf);
            }
        }

        if(FD_ISSET(0, &cpy_fds)){
            //prompt for message
            fgets(send_msg_buf, sizeof(send_msg_buf), stdin);
            int bytes_sent = send(sockfd, send_msg_buf, strlen(send_msg_buf), 0);
            if(bytes_sent == -1){
                perror("recv");
            }
        }
    }
    close(sockfd);
}


//Traverses an addrinfo linked list, stoping at the first one able to connect
//returns socket file descriptor or -1 if it failed
int try_connection(struct addrinfo *const addresses){
    int sockfd = -1;
    struct addrinfo *p;
    for(p = addresses; p != NULL; p = p->ai_next){
        printf("Trying:\n");
        print_addr(p);

        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sockfd < 0){
            sockfd = -1;
            perror("socket()");
            continue;
        }
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) < 0){
            sockfd = -1;
            perror("connect()");
            continue;
        }
        break;
    }
    if(sockfd < 0){
        return -1;
    }
    
    printf("Connection established!\n");
    return sockfd;
}


void print_addr(struct addrinfo *addr){
        char host_buf[100]; 
        char serv_buf[100];
        int rv = getnameinfo(addr->ai_addr, addr->ai_addrlen, host_buf, sizeof(host_buf),
                serv_buf, sizeof(serv_buf), NI_NUMERICHOST | NI_NUMERICSERV);
        if(rv < 0){
            printf("getnameinfo() failed\n");
        }
        printf("%s\n%s\n", host_buf, serv_buf);
}