/*
Il client si connette al server, invia una stringa contenente una sequenza di numeri interi separati
da virgole (esempio: "2, 1, 3, 6, 9, 10, 52"), e riceve dal server la stessa sequenza ordinata in modo
crescente (esempio: "1, 2, 3, 6, 9, 10, 52"). Il server, una volta ricevuta la stringa, esegue il
parsing dei numeri, li ordina, costruisce una nuova stringa con i numeri ordinati e la invia al client.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUFSIZE 1024

int intcmp(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int main() {
    int listenfd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFSIZE];

    // 1. Crea socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Prepara l'indirizzo
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 4. Listen
    listen(listenfd, 5);
    printf("Server in ascolto sulla porta %d...\n", PORT);

    while (1) {
        // 5. Accept
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);
        if (connfd < 0) {
            perror("accept");
            continue;
        }
        printf("Connessione accettata\n");

        // 6. Ricezione dati
        int bytes_read = recv(connfd, buffer, BUFSIZE - 1, 0);
        if (bytes_read <= 0) {
            perror("recv");
            close(connfd);
            continue;
        }

        buffer[bytes_read] = '\0';
        printf("Ricevuto: %s\n", buffer);

        // 7. Parsing
        int *numbers = NULL;
        int count = 0;
        char *token = strtok(buffer, ",");
        while (token != NULL) {
            int num = atoi(token);
            int *tmp = realloc(numbers, (count + 1) * sizeof(int));
            if (!tmp) {
                perror("realloc");
                free(numbers);
                close(connfd);
                continue;
            }
            numbers = tmp;
            numbers[count++] = num;
            token = strtok(NULL, ",");
        }

        if (count == 0) {
            printf("Nessun numero valido ricevuto\n");
            close(connfd);
            continue;
        }

        // 8. Ordinamento
        qsort(numbers, count, sizeof(int), intcmp);

        // 9. Ricostruzione stringa ordinata
        char sorted[BUFSIZE] = {0};
        char temp[32];
        for (int i = 0; i < count; i++) {
            snprintf(temp, sizeof(temp), "%d", numbers[i]);
            strcat(sorted, temp);
            if (i < count - 1) strcat(sorted, ", ");
        }

        // 10. Invio risultato
        send(connfd, sorted, strlen(sorted), 0);
        printf("Inviato: %s\n", sorted);

        // 11. Pulizia
        free(numbers);
        close(connfd);
    }

    close(listenfd);
    return 0;
}
