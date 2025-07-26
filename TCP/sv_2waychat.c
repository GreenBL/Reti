#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>
#define MAX_CLIENTS 100
int check(int value_to_check, char *err_message) {
    if (value_to_check < 0) {
        perror(err_message);
        exit(EXIT_FAILURE);
    }
    return 0;
}


int main() {
    int listenfd, connfd;
    struct sockaddr_in svaddr, claddr;
    int broadcast_in;
    socklen_t svaddr_len = sizeof(svaddr);
    socklen_t claddr_len = sizeof(claddr);
    pid_t pid;
    size_t bytes_read = 0;
    int portno = 12345;
    int internal_port = 12346;
    char buffer[1024];
    int sockfd_arr[100] = {-1};
    pid_t pids[MAX_CLIENTS] = {0};
    int initialized = 0;
    int pipefd[2];

    check(pipe(pipefd), "pipe failed");

    bzero(&svaddr, sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(portno);
    svaddr.sin_addr.s_addr = INADDR_ANY;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    check(listenfd, "Failed to create socket");
    check(bind(listenfd, (struct sockaddr *) &svaddr, svaddr_len), "Failed to bind socket");

    listen(listenfd, 2);


    for (;;) {
        bzero(&claddr, sizeof(claddr));
        claddr_len = sizeof(claddr);
        connfd = accept(listenfd, (struct sockaddr *) &claddr, &claddr_len);
        check(connfd, "Failed to accept connection");

        // add the dude to the client array
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (sockfd_arr[i] == -1) {
                sockfd_arr[i] = connfd;
                break;
            }
        }
        if (initialized == 0) {
            initialized = 1;
            pid = fork();
            if (pid < 0) {
                perror("Failed to fork");
            }
            if (pid == 0) {
                close(listenfd);
                close(pipefd[1]);
                while (1) {
                    bzero(buffer, sizeof(buffer));
                    bytes_read = read(pipefd[0], &buffer, sizeof(buffer));
                    if (bytes_read == 0) {
                        printf("FUCKING ERROR\n");
                        continue;
                    }
                    //buffer[strlen(buffer)] = '\0';
                    printf("[B]: %s\n", buffer);
                    for (int i = 0; i < MAX_CLIENTS; i++) {
                        if (sockfd_arr[i] == -1) {
                            continue;
                        } else {
                            printf("[D]: %d\n", sockfd_arr[i]);
                            snprintf(buffer, sizeof(buffer), "MESSAGE: %s\n", buffer);
                            send(sockfd_arr[i], &buffer, sizeof(buffer), 0);
                        }

                    }
                }
            }
        }


        pid = fork();
        check(pid == -1, "Failed to fork");
        if (pid == 0) {
            close(listenfd);
            close(pipefd[0]);
            // --------------
            bzero(buffer, sizeof(buffer));
            while (strcmp(buffer, "exit") != 0) {
                bzero(buffer, sizeof(buffer));
                bytes_read = recv(connfd, &buffer, sizeof(buffer), 0);
                if (bytes_read == 0) {
                    close(connfd);
                    printf("Connection closed\n");
                    return 0;
                }
                buffer[strlen(buffer) - 1] = '\0';
                write(pipefd[1], buffer, strlen(buffer));
                printf("%s\n", buffer);
            }
            close(connfd);
            return 0;
            // --------------
        }
        close(connfd);
    }

}