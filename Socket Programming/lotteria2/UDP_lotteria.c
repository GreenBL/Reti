#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#define PORT 8080


int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;
    struct sockaddr_in server, client;
    socklen_t cl_len, sv_len;
    char buffer[1024];
    pid_t pid;
    int temp_n;

    bzero(&server, sizeof(server));
    bzero(&client, sizeof(client));

    sv_len = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    listen_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listen_fd < 0) {
        perror("socket");
    }

    if (bind(listen_fd, (struct sockaddr *) &server, sv_len) < 0) {
        perror("bind");
    }

    listen(listen_fd, 10);

    // init random number
    srandom(time(NULL));
    int winning_number = (int) (random() % 1000) + 1;
    printf("init %d\n", winning_number);


    do {
        cl_len = sizeof(client);
        bzero(&buffer, sizeof(buffer));
        recvfrom(listen_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, &cl_len);
        temp_n = atoi(buffer);

        if (temp_n == winning_number) {
            bzero(&buffer, sizeof(buffer));
            snprintf(buffer, sizeof(buffer),"You won the game! Congratulations... you guessed the number: %d\r", winning_number);
            sendto(listen_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, cl_len);
        } else {
            bzero(&buffer, sizeof(buffer));
            snprintf(buffer, sizeof(buffer),"Stupid motherfucker! The number inserted was wrong! Try again next time loser.\n");
            sendto(listen_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, cl_len);
        }


    } while (temp_n != winning_number);

    close(listen_fd);



}