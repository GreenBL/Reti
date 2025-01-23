#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5700
#define MAX_BUFFER_SIZE 1024

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];

    // Creazione del socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    // Configurazione dell'indirizzo del server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding del socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Errore nel binding del socket");
        exit(EXIT_FAILURE);
    }

    // In attesa di connessioni
    if (listen(server_socket, 10) == -1) {
        perror("Errore nell'ascolto delle connessioni");
        exit(EXIT_FAILURE);
    }

    // Accettazione di una connessione
    new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (new_socket == -1) {
        perror("Errore nell'accettazione della connessione");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Ricezione del messaggio dal client
        recv(new_socket, buffer, sizeof(buffer), 0);
        printf("Messaggio dal client: %s\n", buffer);
        if(strcmp(buffer, "STOP\n") == 0) {
            close(new_socket);
            close(server_socket);
            return 0;
        }

        // Invio di un messaggio al client
        printf("Inserisci un messaggio per il client: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(new_socket, buffer, sizeof(buffer), 0);
    }

    close(new_socket);
    close(server_socket);

    return 0;
}
