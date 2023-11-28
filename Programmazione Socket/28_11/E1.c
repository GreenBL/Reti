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
    printf("listening on port: %d", portno);
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
    int counter, n;
    do {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,(socklen_t *) &clilen);

        if(newsockfd < 0) {
            error("Error on accepting connection to client");
        }

        pid = fork();
        if (pid == 0) {
            close(sockfd);
            bzero(buffer,BUFFER_SIZE);

            n = read(newsockfd,buffer,255);

            if (n < 0) {
                error("ERROR reading from socket");
            }

            if(!strcmp(buffer, "T\n")) {
                counter++;
                time_t t = time(NULL);
                char* timestr = ctime(&t);
                n = write(newsockfd, timestr, strlen(timestr)+1);

                if (n < 0) {
                    error("ERROR writing to socket");
                } else if (!strcmp(buffer, "N\n")) {
                    char cnt[5];
                    sprintf(cnt, "%d", counter);
                    n = write(newsockfd, cnt, strlen(cnt));
                    if (n < 0) {
                        error("ERROR writing to socket");
                    } else {
                        // delay
                        printf("Please press a key...");
                        char c;
                        scanf("%c", &c);
                        n = write(newsockfd, "Message received", 20);
                        if (n < 0) {
                        error("ERROR writing to socket");
                        }
                    }   
                }
            }
            close(newsockfd);
        }
        close(newsockfd);
        return 0;

    } while(1);

    


}
