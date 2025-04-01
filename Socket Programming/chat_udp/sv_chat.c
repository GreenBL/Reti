#include "sv_chat.h"

#define MAX_CLIENTS 100  // Numero massimo di client connessi

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
} ClientInfo;

int main() {
    int sockfd, client_len, num_clients = 0;
    struct sockaddr_in server, client;
    ClientInfo clients[MAX_CLIENTS];  // Lista di client connessi
    Message* MSGbuffer = malloc(sizeof(Message));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind");
        exit(1);
    }

    printf("Server ready and listening on port %d\n", PORT);

    while (1) {
        client_len = sizeof(client);
        recvfrom(sockfd, MSGbuffer, sizeof(Message), 0, (struct sockaddr*)&client, (socklen_t*)&client_len);

        // Controlla se il client è già nella lista
        int exists = 0;
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].addr.sin_addr.s_addr == client.sin_addr.s_addr &&
                clients[i].addr.sin_port == client.sin_port) {
                exists = 1;
                break;
            }
        }

        // Se il client non è nella lista, lo aggiunge
        if (!exists && num_clients < MAX_CLIENTS) {
            clients[num_clients].addr = client;
            clients[num_clients].addr_len = client_len;
            num_clients++;
        }

        printf("<%s>: %s\n", MSGbuffer->sender, MSGbuffer->content);

        // Invia il messaggio a tutti i client (escluso il mittente)
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].addr.sin_addr.s_addr == client.sin_addr.s_addr &&
                clients[i].addr.sin_port == client.sin_port) {
                continue;  // Non inviare il messaggio al mittente
            }
            sendto(sockfd, MSGbuffer, sizeof(Message), 0,
                   (struct sockaddr*)&clients[i].addr, clients[i].addr_len);
        }

        bzero(MSGbuffer, sizeof(Message));
    }

    close(sockfd);
    free(MSGbuffer);
    return 0;
}
