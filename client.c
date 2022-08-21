#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

#define MAX_INPUT_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    char buffer[MAX_INPUT_SIZE];
    if (argc < 3) {
       fprintf(stderr,"usage %s <server-ip-addr> <server-port>\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);

    /* Create client socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket\n");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if(!inet_aton(argv[1], &serv_addr.sin_addr)) {
        fprintf(stderr, "ERROR invalid server IP address\n");
        exit(1);
    }
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Connected to server\n");

    while(1) {
        printf("enter command: ");
        bzero(buffer,MAX_INPUT_SIZE);
	    fgets(buffer,MAX_INPUT_SIZE-1,stdin);

        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");

        if(buffer[0]=='q') {
            close(sockfd);
            printf("disconnected from server\n");
            exit(1);
        }

        bzero(buffer,MAX_INPUT_SIZE);
        n = read(sockfd,buffer,MAX_INPUT_SIZE-1);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("server: %s\n",buffer);

    } 

    return 0;
}

