#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]) {
    struct sockaddr_in sv_addr;
    char sendbuff[1024];
    char messagebuff[1024];
    char username[1024];
    int usersize;
    int msgsize;

    const int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&sv_addr, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(8080);

    printf("Connecting to server...\n");

    printf("Please enter your username: ");
    fflush(stdout);
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    if (connect(sock_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    printf("Connected to server!\n");

    do {
        printf("Please enter your message: ");
        bzero(sendbuff, sizeof(sendbuff));
        bzero(messagebuff, sizeof(messagebuff));
        fgets(messagebuff, sizeof(sendbuff), stdin);

        messagebuff[strcspn(messagebuff, "\n")] = 0;
        sprintf(sendbuff, "[%s]: %s", username ,messagebuff);
        printf("This is what are you sending: %s\n", sendbuff);
        int check = send(sock_fd, &sendbuff, strlen(sendbuff), 0);
        if (check < 0) {
            perror("send failed");
            close(sock_fd);
            exit(1);
        }
    } while (sendbuff[0] != 'E');

}


