#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// Socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

// Buffer
#define BUFFER_SIZE 256

void error(char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[]) {
    int sockfd;
    int port;
    int client_socket_size; 
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];

    // hostname e port number
    if(argc < 3) {
        fprintf(stderr,"Usage %s [PORT NUMBER]", argv[1]);
        error("\n");
    }

    port = atoi(argv[2]);

    // Creazione della socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error("\n! SOCKET CREATION ERROR !\n");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL) {
        error("\n! ERROR: HOST NOT FOUND !");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port); 

    // Connessione

    int flag = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(flag < 0) {
        error("! ERROR CONNECTING TO ADDRESS !");
    }

    // Operazioni client
    do {
        bzero(buffer, sizeof(buffer));
        printf("\nEnter message: ");
        fgets(buffer, 255, stdin);
        flag = write(sockfd, buffer, strlen(buffer));
        if(flag < 0) {
            error("! ERROR WRITING TO SOCKET !");
        }
        if(strcmp(buffer, "END\n") == 0) {
            close(sockfd);
            return 0;
        }
    } while(1);
    close(sockfd);
    
    return 0;


}