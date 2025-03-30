#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

typedef struct client {
  char username[33];
  int client_socket;
} Client;

int main(int argc, const char * argv[]) {
  int client_fd;
  struct sockaddr_in sv_addr;
  char *username;
  size_t username_len = 0;
  Client local_client;
  pid_t pid;
  char buffer[BUFSIZ];
  bcopy("__NIL", &local_client.username, sizeof(local_client.username));

  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&sv_addr, sizeof(sv_addr));
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(PORT);
  sv_addr.sin_addr.s_addr = INADDR_ANY;

  // BINDING
  // if (bind(client_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) == -1) {
  //   perror("bind");
  //   exit(1);
  // }

  if (connect(client_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) == -1) {
    perror("error connecting to server");
    exit(1);
  }

  printf("Connected to dedicated dice roll server. Please insert a username:\n");

  while (strcmp((local_client.username), "__NIL") == 0) {
    getline(&username, &username_len, stdin);
    if (strlen(username) > 32) {
      printf("Username too long\n");
      continue;
    }
    bzero(&local_client.username, sizeof(local_client.username));
    bcopy(username, &local_client.username, sizeof(local_client.username));
    local_client.username[strcspn(local_client.username, "\n")] = '\0';
  }
  free(username);

  printf("Username: %s\n", local_client.username);

  // Introducing the client to the server
  send(client_fd, &local_client, sizeof(local_client), 0);
  // Forking the workload to always listen for incoming messages
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    // parent
    do {
      printf("Type 'R' to roll the dice! Type \"exit\" to exit.\n");
      fgets(buffer, sizeof(buffer), stdin);
      if (strcmp(buffer, "R\n") == 0) {
        send(client_fd, "roll", strlen("roll"), 0);
      } else if (strcmp(buffer, "exit\n") == 0) {
        printf("Ok pal.\n");
        exit(0);
      }
    } while (1);

  } else {
    // child
    while (getpid() != 1) {
      recv(client_fd, &local_client.username, sizeof(local_client.username), 0);
      printf("%s\n", buffer);
    }
  }

  return 0;
}