#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    char *default_response = "Hello, World!";
    char response[1024];
    int sockfd, newsockfd, clilen;
    int portno = 8080;
    struct sockaddr_in serv_addr, cli_addr;

    time_t t;
    struct tm *local;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    listen(sockfd, 5);

    do {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0) {
            perror("Error on accept.");
            exit(1);
        }
        // Get the current time before sending to socket
        time(&t);
        local = localtime(&t);
        // Format the string and print to buffer
        bzero(response, sizeof(response));
        sprintf(response, "The current time is: %02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);
        send(newsockfd, response, strlen(response), 0);
        close(newsockfd);
        return 0;
    } while(1);

}
