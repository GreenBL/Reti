/* Realizzare un'applicazione TCP per la realizzazione di una chat. L'applicazione deve consentire sia al client che al server di mandare messaggi e di visualizzare i messaggi ricevuti, secondo uno schema alternato rigido (client-server-client-server) */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>       
#include <unistd.h>
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h>

#define BUFFER_SIZE 256

void error(char *msg_s) {
    perror(msg_s);
    exit(1);
} 

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int portno, clilen;
    char buffer[256];

    // argument count check
    if(argc < 2) {
        fprintf(stderr, "argc < 2");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    struct sockaddr_in serv_addr, cli_addr;

    bzero((char*) &serv_addr , sizeof(serv_addr));

    portno = atoi(argv[1]); // converts port number from string to int
    printf("listening on port: %d\n", portno);
    // server addr init
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // bind socket + check
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Error on binding");
    }

    // listen socket

    listen(sockfd, 5);

    // estabilish connection with client
    pid_t pid;
    int counter, condition_check;
    int loop;
    do {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,(socklen_t *) &clilen);

        if(newsockfd < 0) {
            error("Error on accepting connection to client");
        }

        pid = fork();

        if (pid == 0) {
            // read socket data
            bzero(buffer, sizeof(buffer));
            condition_check = read(newsockfd, buffer, sizeof(buffer) - 1);
            printf("I have read from socket: %s\n", buffer);
            close(sockfd);
            close(newsockfd);
            return 0;
        }
        
        close(newsockfd);
        printf("Do you want to terminate the parent server process?\n");
        bzero(buffer,sizeof(buffer));
        fgets(buffer, sizeof(buffer) - 1, stdin);
        condition_check = strcmp(buffer, "yes");
        if(condition_check == 0) {
            loop = 0;
        }

    } while(loop == 1);

    return 0;


}
