#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 8000

typedef struct Host {
    struct sockaddr_in addr;
    int sock;
} Host;

typedef struct HostList {
    Host *host;
    struct HostList *next;
} HostList;

int main(int argc, char *argv[]) {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pid_t pid;

    HostList *head = NULL;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 5);

    while (1) {
        printf("Waiting to accept...\n");
        clilen = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);

        // Insert at head
        HostList *new_node = (HostList *)malloc(sizeof(HostList));
        new_node->host = (Host *)malloc(sizeof(Host));
        new_node->host->addr = cli_addr;
        new_node->host->sock = connfd;
        new_node->next = head;
        head = new_node;

        pid = fork();
        if (pid == 0) {
            // Child process
            close(listenfd); // Close listening socket in child
            printf("Handling client...\n");

            // Example: simple echo
            char buffer[1024];
            int bytes;
            while ((bytes = read(connfd, buffer, sizeof(buffer))) > 0) {
                write(connfd, buffer, bytes);
            }

            close(connfd);
            exit(0);
        } else {
            // Parent process
            close(connfd); // Parent closes connected socket
        }
    }
}
