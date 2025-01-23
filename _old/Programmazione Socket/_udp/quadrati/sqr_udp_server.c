#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>     /* per strlen() */
#include <strings.h>    /* per bzero() e bcopy() */

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>  /* tipi di dati di sistema */
#include <sys/socket.h> /* definizioni utili per le socket() */
#include <arpa/inet.h>

void error(char msg[]) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd;
    int port;
    int check;
    socklen_t clilen;
    int bytesRead;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[BUFSIZ], readLine[BUFSIZ];

    if(argc < 2) {
        fprintf(stderr, "No port given");
        exit(1);
    }

    // Buffers init
    bzero(buffer, sizeof(buffer));
    bzero(readLine, sizeof(readLine));

    // Grab the port number from args
    port = atoi(argv[1]);

    // Socket init
    bzero((char*) &serv_addr, sizeof(serv_addr));
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(sockfd < 0) {
        error("Invalid socket file descriptor");
    }
    
    check = bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(check < 0) {
        error("Error on bind");
    }
    int exitCondition = 0;
    clilen = sizeof(cli_addr);
    for(;;) {
        // ------------------ 'DEBUG'

        //fprintf(stdout, "%s", buffer);

        // ------------------ END DEBUG
        printf("Waiting for packet\n");
        bzero(readLine, BUFSIZ);
        bytesRead = recvfrom(sockfd, readLine, BUFSIZ, 0, (struct sockaddr*) &cli_addr, &clilen);
        // debug
        char* ipString = inet_ntoa(cli_addr.sin_addr);
        fprintf(stdout, "\nAddress > %s\n", ipString);
        readLine[bytesRead] = '\0'; // null terminate the string 
        exitCondition = strcmp("close\n", readLine);
        
        if(exitCondition == 0) {
            break;
        }
        
        printf("Received Message << %s\n", readLine);

        printf("What do you want to send over? \n>> ");
        
        // Flush buffer
        bzero(buffer, BUFSIZ);
        // Get string from user to send over
        fgets(buffer, BUFSIZ, stdin);
        // am i doing this correctly?
        //fprintf(stdout, "\nYou have inserted the string: %s\n", buffer);
        // Send string to user
        if(sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*) &cli_addr, sizeof(cli_addr)) < 0){
            error("ERROR : sendto()");
        }
        // Restart the loop
        //printf("\n\tdebug delay\n");
        //scanf("\n");

    }
    printf("program terminated as expected\n");
    close(sockfd);
    exit(1);

}