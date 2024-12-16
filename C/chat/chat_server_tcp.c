#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
// remove from mac
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int socketfd;
    int newsockfd;
    int portno;
    int clilen;
    pid_t pid;
    int n; // write bytes
    char readbuffer[256];
    int status;

    struct sockaddr_in cl_addr;
    struct sockaddr_in sv_addr;

    // main socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        error("Socket Error\n");
    }

    bzero((char*) &cl_addr, sizeof(cl_addr)); // doesnt matter since its calculated
    bzero((char*) &sv_addr, sizeof(sv_addr));

    portno = 9000;
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(portno);

    int bind_result = bind(socketfd, (struct sockaddr*) &sv_addr, sizeof(sv_addr));
    if (bind_result < 0) {
        error("Could not bind socket struct to file descriptor.\n");
    }

    int queue_len = 5;
    listen(socketfd, queue_len);

    // accept loop

    do {
        printf("preparing to accept a new client\n");
        clilen = sizeof(cl_addr);
        printf("Listening...\n");
        newsockfd = accept(socketfd,(struct sockaddr*) &cl_addr, (socklen_t*) &clilen);
        printf("New connection from client.\n");

        if (newsockfd < 0) {
            error("Error on accepting client./n");
        }

        if ((pid = fork()) < 0) {
            error("Forking process error.\n");
        }

        /* CHILD PROCESS CODEBLOCK */
        if (pid == 0) {
            close(socketfd); // closing main socket on child process
            bzero(readbuffer, sizeof(readbuffer));

            n = read(newsockfd, readbuffer, 255);
            if (n < 0) {
                error("Error reading data from socket.\n");
            }

            printf("new user connected: %s\n", readbuffer);

        }
        /* END CHILD PROCESS CODEBLOCK */

    } while (1);

    wait(&status);


}
