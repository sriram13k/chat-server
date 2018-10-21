#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

#define BACKLOG 5
#define MSG_SIZE 256

void error(const char* error_msg)
{
    perror(error_msg);
    exit(1);
}


int main(int argc, char* argv[])
{
    int sockfd, new_fd , portno;
    socklen_t client_length;
    char buffer[MSG_SIZE];
    struct sockaddr_in serv_addr,client_addr;

    if( argc < 2 ) {
        fprintf(stderr, " usage %s portno\n", argv[0]);
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if( sockfd < 0 ) {
        error("ERROR opening socket");
    }

    memset(&serv_addr , 0 , sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr* ) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd,BACKLOG);
    client_length = sizeof(client_addr);
    new_fd = accept(sockfd, (struct sockaddr *) &client_addr, &client_length);

    if (new_fd < 0) {
        error("ERROR on accept");
    }

    while (true) {

        memset(buffer, 0 , MSG_SIZE);
        if( read(new_fd, buffer , MSG_SIZE) < 0) {
            error("ERROR reading from the socket");
        }
        printf("Client : %s", buffer);
        memset(buffer,0,MSG_SIZE);
        fgets(buffer,MSG_SIZE-1,stdin);
        if( write(new_fd,buffer,strlen(buffer)) < 0) {
            error("ERROR writing to the socket");
        }

    }

    close(new_fd);
    close(sockfd);
    return 0;

}
