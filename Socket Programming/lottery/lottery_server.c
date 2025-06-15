#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 12345
#define MAX_ATTEMPTS 3
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in addr;
    int attempts;
} ClientInfo;

ClientInfo clients[FD_SETSIZE];
int client_count = 0;

int find_client(struct sockaddr_in *addr) {
    for (int i = 0; i < client_count; i++) {
        if (memcmp(&clients[i].addr, addr, sizeof(struct sockaddr_in)) == 0) {
            return i;
        }
    }
    return -1;
}

void add_client(struct sockaddr_in *addr) {
    if (client_count < FD_SETSIZE) {
        clients[client_count].addr = *addr;
        clients[client_count].attempts = 0;
        client_count++;
    }
}

void remove_client(int index) {
    if (index >= 0 && index < client_count) {
        clients[index] = clients[client_count - 1];
        client_count--;
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);
    int secret_number;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel binding");
        exit(1);
    }

    printf("Server UDP in ascolto sulla porta %d\n", PORT);
    srand(time(NULL));
    secret_number = rand() % 100 + 1;
    printf("Secret: %d\n", secret_number);

    while (1) {
        int recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("Errore nella ricezione");
            continue;
        }
        buffer[recv_len] = '\0';

        int client_index = find_client(&client_addr);
        if (client_index == -1) {
            add_client(&client_addr);
            client_index = client_count - 1;
        }

        int guess = atoi(buffer);
        clients[client_index].attempts++;

        if (guess == secret_number) {
            char win_msg[] = "Hai indovinato! Congratulazioni!";
            sendto(sock, win_msg, strlen(win_msg), 0, (struct sockaddr*)&client_addr, addr_len);
            remove_client(client_index);
        } else if (clients[client_index].attempts >= MAX_ATTEMPTS) {
            char lose_msg[] = "Hai esaurito i tentativi. Disconnesso.";
            sendto(sock, lose_msg, strlen(lose_msg), 0, (struct sockaddr*)&client_addr, addr_len);
            remove_client(client_index);
        } else {
            char try_again_msg[] = "Tentativo errato. Ritenta.";
            sendto(sock, try_again_msg, strlen(try_again_msg), 0, (struct sockaddr*)&client_addr, addr_len);
        }
    }

    close(sock);
    return 0;
}
