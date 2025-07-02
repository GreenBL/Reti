#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define BUFSIZE 1024
#define PORT 8080

int client_handler(int sockfd) {
    char buffer[BUFSIZE];
    size_t bufsize = sizeof(buffer);
    int bytes;
    bzero(buffer, BUFSIZE);
    snprintf(buffer, BUFSIZE, "Welcome nigger!\n");
    write(sockfd, buffer, strlen(buffer));
    while (bytes > 0) {
        bzero(buffer, BUFSIZE);
        read(sockfd, buffer, BUFSIZE);
        printf("%s", buffer);
        fgets(buffer, BUFSIZE, stdin);
        write(sockfd, buffer, strlen(buffer));
    }
    return 0;
}

void die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len, server_addr_len;
    client_addr_len = sizeof(client_addr);
    server_addr_len = sizeof(server_addr);
    pid_t pid;
    int binding_result;
    int bytes_generic = 0;

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        die("Socket error!");
    }

    binding_result = bind(listen_fd, (struct sockaddr *) &server_addr, server_addr_len);
    if (binding_result < 0) {
        die("Bind error!");
    }

    listen(listen_fd, 5);

    do {
        // accept incoming connections
        client_addr_len = sizeof(client_addr);
        conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (conn_fd < 0) {
            die("Accept error!");
        }

        pid = fork();
        if (pid < 0) {
            die("Fork error!");
        }
        if (pid == 0) {
            close(listen_fd);
            client_handler(conn_fd);
        }
        close(conn_fd);
        bytes_generic = 1;
    } while (bytes_generic > 0);


}