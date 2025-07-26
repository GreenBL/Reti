#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

typedef struct Node {
    struct sockaddr_in host_addr;
    struct Node *next;
} Node;

Node *createNode(struct sockaddr_in host_addr) {
    Node *node = malloc(sizeof(Node));
    node->host_addr = host_addr;
    node->next = NULL;
    return node;
}

void freeList(Node *node) {
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
}

void pdie(const char *message) {
    perror(message);
    exit(1);
}

void assertf(const int condition, const char *message) {
    if (condition < 0) {
        fprintf(stderr, "Assertion '%s' failed, at %d.\n", message, condition);
        fflush(stderr);
        pdie(message);
    }
}

int main() {
    int listenfd, connfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t c_len, s_len;
    char stdbuff[1024];
    char auxbuff[1024];
    struct Node *head = NULL;
    int eof = 0;
    pid_t pid;

    bzero(&server, sizeof(server));
    bzero(&client, sizeof(client));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(12345);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assertf(listenfd >= 0, "socket() failed");
    assertf(bind(listenfd, (struct sockaddr *) &server, sizeof(server)), "Error on binding.");

    listen(listenfd, 5);
    printf("Listening...\n");
    do {
        c_len = sizeof(client);
        connfd = accept(listenfd, (struct sockaddr *) &client, &c_len);
        assertf(connfd, "accept() failed");
        if (head == NULL) {
            head = createNode(client);
        } else {
            Node *node = head;
            while (node->next != NULL) {
                node = node->next;
            }
            node->next = createNode(client);
        }
        pid = fork();
        assertf(pid >= 0, "fork() failed");
        if (pid == 0) {
            close(listenfd);
            // -------------------------------------------
            {
                bzero(stdbuff, sizeof(stdbuff));
                recv(connfd, &stdbuff, sizeof(stdbuff), 0);
                printf("[DEBUG] %s\n", stdbuff);
                char *str2 = stdbuff;
                for (int i = 0; str2[i] != '\0'; i++) {
                    str2[i] = toupper(str2[i]);
                    printf("%c", str2[i]);
                    printf("string is %d long at this cycle\n", i);
                }
                send(connfd, str2, strlen(str2), 0);
            }
            // -------------------------------------------
            freeList(head);
            close(connfd);
            return 0;
        }
    close(connfd);

    } while (1);

}