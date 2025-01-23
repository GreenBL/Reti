#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

// Socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Buffer
#define BUFFER_SIZE 256

void error(char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int port;
    int client_socket_size; 
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    char buffer[BUFFER_SIZE];
    pid_t pid;
    int status;

    // Port number from argv[1]?
    if(argc < 2) {
        printf("Usage %s [PORT NUMBER]", argv[0]);
        error("\n");
    }

    port = atoi(argv[1]);

    // Creazione della socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error("\n! SOCKET CREATION ERROR !\n");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Binding della socket

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int flag = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(flag < 0) {
        error("! ERROR BINDING SOCKET !");
    }

    // Loop handling
    int end = -1;
    // Listen
    listen(sockfd, 5);

    // Operazioni server
    do {
        // Accetta la connessione in arrivo
        client_socket_size = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &client_socket_size);
        if(newsockfd < 0) {
            error("! ERROR ACCEPTING CONNECTION !");
        }

        // Processo figlio
        pid = fork();

        if(pid == 0) {
            do {
                bzero(buffer, sizeof(buffer));
                flag = read(newsockfd, buffer, 255);
                if(flag < 0) {
                    error("! ERROR READING FROM SOCKET !");
                }
                if(strcmp(buffer, "END\n") == 0) {
                    return 0;
                }
                printf("\nMessage received: %s", buffer);
                } while(1);
            close(newsockfd);
            return 0;
        }

        wait(&status);
        printf("\nEnter -1 to end, 0 to continue: ");
        scanf("%d", &end);

        // todo - debug stuff here
        close(newsockfd);

    } while(end >= 0);

    close(sockfd);

    return 0;

}

