#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>

#define D_SIZE 20
#define PORT 8080

int roll() {

  srandom(time(NULL));
  const int d = (int) random() % D_SIZE; // 0 - 19 > 1 - 20
  return d + 1;
}

typedef struct client {
  char username[33];
  int client_socket;
} Client;

int main(int argc, char *argv[]) {
  int socket_fd, client_fd, client_len;
  struct sockaddr_in sv_addr, cl_addr;
  pid_t pid;
  Client clients[16];
  int connected_clients = 0;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("socket");
    exit(1);
  }
  bzero(&sv_addr, sizeof(sv_addr));
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(PORT);
  sv_addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(socket_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0) {
    perror("bind");
    exit(1);
  }
  listen(socket_fd, 5);
  printf("Listening on port %d\n", PORT);
  do {
    client_len = sizeof(cl_addr);
    client_fd = accept(socket_fd, (struct sockaddr *)&cl_addr, (socklen_t *)&client_len);
    printf("Client connected from %s\n", inet_ntoa(cl_addr.sin_addr));
    if (client_fd < 0) {
      perror("ACCEPT ERROR");
      exit(1);
    }
    connected_clients++; // Increase the connected clients number

    pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(1);
    }
    if (pid == 0) {
      // Child process - interfaces with single clients
      //close(socket_fd);
      recv(client_fd, &clients[connected_clients], sizeof(clients[connected_clients]), 0);
      char welcome_str[128];
      sprintf(welcome_str, "Welcome to D20 %s", clients[connected_clients].username);
      send(client_fd, welcome_str , sizeof(clients[connected_clients]), 0);
      // DOES THE CLIENT WANT TO SUFFER?
    } else {
      // Parent process - global scope
      //close(client_fd);
      Client curr_client;
      bzero(&curr_client, sizeof(curr_client));
      curr_client.client_socket = client_fd;
      clients[connected_clients] = curr_client;
      connected_clients++;
    }
    close(socket_fd);
    close(client_fd);

  } while (1);

  return 0;
}