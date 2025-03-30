#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080

int main (int argc, char *argv[]) {
  int main_fd, client_fd, clilen;
  struct sockaddr_in sv_addr, cl_addr;
  pid_t pid;
  int connection_counter = 0;
  char buffer[1024];
  int bytes;

  bzero(&sv_addr, sizeof(sv_addr));
  bzero(&cl_addr, sizeof(cl_addr));
  main_fd = socket(AF_INET, SOCK_STREAM, 0);
  sv_addr.sin_family = AF_INET;
  sv_addr.sin_port = htons(PORT);
  sv_addr.sin_addr.s_addr = INADDR_ANY;

  // Binding of sv_addr
  if (bind(main_fd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0) {
    perror("bind failed");
    exit(1);
  }

  listen(main_fd, 5);

  do {
    clilen = sizeof(cl_addr);
    client_fd = accept(main_fd, (struct sockaddr *)&cl_addr, (socklen_t *)&clilen);

    if (client_fd < 0) {
      perror("accept failed");
      exit(1);
    }
    pid = fork();
    if (pid == 0) {
      // Child process //
      // ReSharper disable once CppDFAEndlessLoop
      while (buffer[0] != 'E') {
        bzero(buffer, sizeof(buffer));
        bytes = (int) recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes < 0 || bytes == 0) {
          printf("Connection closed\n");
          exit(0);
        }
        printf("%s\n", buffer);
        sleep(1);
      }
      exit(0);
    } else {
      // Parent process //
      printf("Parent %d\n", getpid());
      connection_counter++;
      printf("Connection number: %d\n", connection_counter);
      if (connection_counter == 10) {
        break;
      }
    }
  } while(1);
  wait(NULL);
  close(main_fd);
  return 0;
}