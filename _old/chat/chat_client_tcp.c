#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <string.h>
#include <strings.h>    /* bzero(), bcopy() */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define HOSTNAME "localhost"
#define PORT "9000"

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in sv_addr;
    struct hostent *server;
    char buff[256];
    char username[16];

    if (argc < 3) {
       fprintf(stderr,"usage (%s <hostname> <port>)\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Socket error.\n");
    }

    if (server == NULL) {
        error("No such host.\n");
    }

    printf("It's over\n");

    bzero((char*) &sv_addr, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &sv_addr.sin_addr.s_addr, server->h_length);
    sv_addr.sin_port = htons(portno);

    int connection_result = connect(sockfd, (struct sockaddr*) &sv_addr  , sizeof(sv_addr));
    if (connection_result < 0) {
        error("Could not connect to host.\n");
    }

    printf("Please enter your username: ");
    bzero(buff, sizeof(buff));
    fgets(buff, 255, stdin);

    n = write(sockfd, buff, sizeof(buff));

    if (n < 0) {
        error("Error writing to socket.");
    }

    close(sockfd);
}
