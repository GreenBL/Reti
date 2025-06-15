#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
	int sock;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];
	socklen_t addr_len = sizeof(server_addr);

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Errore nella creazione del socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	printf("Benvenuto alla lotteria UDP! Hai 3 tentativi per indovinare il numero.\n");

	for (int i = 0; i < 3; i++) {
		printf("Inserisci un numero: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strcspn(buffer, "\n")] = 0;

		sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len);

		int recv_len = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
		if (recv_len < 0) {
			perror("Errore nella ricezione");
			exit(1);
		}
		buffer[recv_len] = '\0';
		printf("Risposta del server: %s\n", buffer);

		if (strstr(buffer, "Congratulazioni") || strstr(buffer, "Disconnesso")) {
			break;
		}
	}

	close(sock);
	return 0;
}
