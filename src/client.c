#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

#define MSG_SIZE 256

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char* argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[MSG_SIZE];

    if( argc < 3 ) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket( AF_INET, SOCK_STREAM, 0);

    if( sockfd < 0 ) {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);
    if ( server == NULL ) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    memset( (char *)&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); 
    //memmove((char*)&serv_addr.sin_addr.s_addr, (char*)&server->h_addr , server->h_length);
    serv_addr.sin_port = htons(portno);

    if( connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    printf("Client: ");
    while(true)
    {
        memset(buffer,0,MSG_SIZE);
        fgets(buffer,MSG_SIZE-1,stdin);
        if( write(sockfd, buffer, strlen(buffer)) < 0 ) {
            error("ERROR writing to socket");
        }
        memset(buffer,0,MSG_SIZE);
        if ( read(sockfd, buffer, MSG_SIZE) < 0) {
            error("ERROR reading from socket");
        }
        printf("Server: %s", buffer);
    }
    close(sockfd);
    return 0;

}
