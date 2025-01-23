#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5700
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Creazione del socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    // Configurazione dell'indirizzo del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Indirizzo IP del server
    server_addr.sin_port = htons(PORT);

    // Connessione al server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nella connessione al server");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Invio di un messaggio al server
        printf("Inserisci un messaggio per il server: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, sizeof(buffer), 0);

        // Ricezione del messaggio dal server
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Messaggio dal server: %s\n", buffer);
        if(strcmp(buffer, "STOP\n") == 0) {
            close(client_socket);
            return 0;
        }
    }
    close(client_socket);
    return 0;
}
