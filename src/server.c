#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define BACKLOG 10

void error(const char* error_msg)
{
    perror(error_msg);
    exit(1);
}



int main(int argc, char* argv[])
{
    
}
