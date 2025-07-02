// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define LOG_FILE "chat.log"
#define MAX_MSG_LEN 1024
#define MAX_CLIENTS 100

typedef struct {
    struct sockaddr_in addr;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

int is_same_client(struct sockaddr_in *a, struct sockaddr_in *b) {
    return a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port;
}

void add_client(struct sockaddr_in *client_addr) {
    for (int i = 0; i < client_count; ++i) {
        if (is_same_client(&clients[i].addr, client_addr))
            return; // già noto
    }
    if (client_count < MAX_CLIENTS) {
        clients[client_count++].addr = *client_addr;
        printf("Nuovo client aggiunto: %s:%d\n",
               inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    }
}

void log_message(const char *msg) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp) {
        fputs(msg, fp);
        fputc('\n', fp);
        fclose(fp);
    }
}

void send_history(int sockfd, struct sockaddr_in *client_addr, socklen_t addrlen) {
    FILE *fp = fopen(LOG_FILE, "r");
    if (!fp) return;

    char line[MAX_MSG_LEN];
    while (fgets(line, sizeof(line), fp)) {
        sendto(sockfd, line, strlen(line), 0, (struct sockaddr *)client_addr, addrlen);
        usleep(50000);
    }

    fclose(fp);
}

void broadcast_message(int sockfd, const char *msg, struct sockaddr_in *sender) {
    for (int i = 0; i < client_count; ++i) {
        if (!is_same_client(&clients[i].addr, sender)) {
            sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
}

int main() {
    int sockfd;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket fallita");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind fallita");
        close(sockfd);
        exit(1);
    }

    printf("Server UDP in ascolto sulla porta %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);

        add_client(&client_addr);

        if (strncmp(buffer, "/history", 8) == 0) {
            send_history(sockfd, &client_addr, addr_len);
        } else {
            log_message(buffer);
            printf("Messaggio ricevuto: %s\n", buffer);
            broadcast_message(sockfd, buffer, &client_addr);
        }
    }

    close(sockfd);
    return 0;
}
