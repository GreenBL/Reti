#include "sv_chat.h"

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server;
    pid_t pid;
    Message* MSGBuffer = malloc(sizeof(Message));
    Message* sendbuffer = malloc(sizeof(Message));
    char content_buffer[1024];
    char username[32];

    printf("Enter username: ");
    fgets(username, 32, stdin);
    username[strcspn(username, "\n")] = 0;  // Rimuove il newline
    strcpy(sendbuffer->sender, username);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    pid = fork();
    if (pid == 0) {
        // Process child -- listening for incoming messages
        while (1) {
            recvfrom(sockfd, MSGBuffer, sizeof(Message), 0, NULL, NULL);

            // Clear the current line in terminal before printing
            printf("\r\033[K");
            printf("<%s>: %s\n", MSGBuffer->sender, MSGBuffer->content);  // Print received message
            printf("<<YOU>>: ");  // Print the prompt again
            fflush(stdout);  // Ensure everything is printed immediately
        }
    }

    // Process parent -- sending messages
    while (1) {
        bzero(content_buffer, sizeof(content_buffer));
        printf("<<YOU>>: ");
        fgets(content_buffer, sizeof(content_buffer), stdin);
        content_buffer[strcspn(content_buffer, "\n")] = 0;  // Rimuove il newline
        strcpy(sendbuffer->content, content_buffer);

        // Send the message to the server
        sendto(sockfd, sendbuffer, sizeof(Message), 0, (struct sockaddr*)&server, sizeof(server));
    }

    close(sockfd);
    free(MSGBuffer);
    free(sendbuffer);
    return 0;
}
