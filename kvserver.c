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
#define MAXSZ 1000

void error(char *msg)
{
    perror(msg);
    exit(1);
}

typedef struct kv {  
    int key;
    int value;
} node;

node *store[MAXSZ];

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
    node *no;

    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        printf("connected to client from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        
        if((childpid = fork()) == 0) {
            close(sockfd);
            while(1) {
                bzero(buffer,MAX_INPUT_SIZE);
                n = read(newsockfd,buffer,MAX_INPUT_SIZE-1);
                if (n < 0) error("ERROR reading from socket");
                if(buffer[0]=='q') {
                    printf("disconnected to client from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    break;
                } else if (buffer[0]=='p') {
                    printf("Client: %s\n",buffer);
                    char * token = strtok(buffer, " ");
                    char * command = token;
                    token = strtok(NULL, " ");
                    char * key = token;
                    token = strtok(NULL, " ");
                    char * value = token;
                    
                    
                    no = (node *)malloc(sizeof(node));
                    int k = atoi(key);
                    no->value = atoi(value);
                    if (store[k] == NULL) {
                        store[k] = no;
                        n = write(newsockfd,"logged",6);
                        // bzero(buffer,MAX_INPUT_SIZE);
                    }
                    else {
                        n = write(newsockfd,"key existed",12);
                        // bzero(buffer,MAX_INPUT_SIZE);
                    }
                } else if (buffer[0]=='g') {
                    printf("Client: %s\n",buffer);
                    char * token = strtok(buffer, " ");
                    char * command = token;
                    token = strtok(NULL, " ");
                    char * key = token;

                    int k = atoi(key);
                    if (store[k] == NULL) {
                        n = write(newsockfd,"key not existed",16);
                    } else {
                        no = store[k];
                        printf("value: %d \n", no->value);
                        snprintf(buffer,MAX_INPUT_SIZE, "%d", no->value);
                        n = write(newsockfd,buffer,MAX_INPUT_SIZE-1);
                        bzero(buffer,MAX_INPUT_SIZE);
                    }
                } else if (buffer[0]=='d') {
                    printf("Client: %s\n",buffer);
                    char * token = strtok(buffer, " ");
                    char * command = token;
                    token = strtok(NULL, " ");
                    char * key = token;
                    
                    int k = atoi(key);
                    store[k] = NULL;
                    n = write(newsockfd,"deleted",7);
                }
            }

        }
        
    } 
    return 0;
}