/*
    @raft implementation

    status  :
        consensus   :
            skeleton    :   done
            inits       :   in progress
            functions   :   inactive
        socket messaging   :
            client      :   inactive
            server      :   inactive
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];

    /*
    struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
    };
    This structure is defined in <netinet/in.h>.
    */

    struct sockaddr_in serv_addr, cli_addr;
    
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    error("ERROR opening socket");

    // set all values in a buffer to zero
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // the IP address of the machine on which the server is running
    serv_addr.sin_port = htons(portno);     // convert portno to network byte order

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR on binding");

    listen(sockfd,5);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    do {
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Client: %s\n",buffer);

        // @unilateral
        n = write(newsockfd,"logged",6);

        /* @bilateral
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(newsockfd,buffer,strlen(buffer));
        */

        if (n < 0) error("ERROR writing to socket");
        
    } while (!(buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D'));
    close(newsockfd);
    close(sockfd);
    return 0; 
}
