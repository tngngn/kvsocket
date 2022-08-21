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
#define PORT 4444

void error(char *msg)
{
    perror(msg);
    exit(1);
}
/*
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Client: %s\n",buffer);
   if(strcmp(buffer, "exit")==0) {
        printf("disconnected from client");
        exit(1);
    }
   n = write(sock,"logged",6);
   if (n < 0) error("ERROR writing to socket");
}
*/
int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    char buffer[MAX_INPUT_SIZE];
    pid_t childpid;
    

    int n;
    
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // the IP address of the machine on which the server is running
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    printf("bind to port %d\n", portno);
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        printf("connected to client from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
/*
        if (newsockfd < 0) 
            error("ERROR on accept");
        pid = fork();
        printf("%d\n",pid);
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)  {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
     else close(newsockfd);
     */
        
        if((childpid = fork()) == 0) {
            close(sockfd);
            while(1) {
                bzero(buffer,MAX_INPUT_SIZE);
                n = read(newsockfd,buffer,MAX_INPUT_SIZE-1);
                if (n < 0) error("ERROR reading from socket");
                
                if(buffer[0]=='q') {
                    printf("disconnected to client from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    break;
                } else {
                    printf("Client: %s\n",buffer);
                    n = write(newsockfd,buffer,MAX_INPUT_SIZE-1);
                    bzero(buffer,MAX_INPUT_SIZE);
                }
            }

        }
        
    } 
    return 0;
}

