#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

typedef struct Node {
    struct sockaddr_in address;
    int socket;
    struct Node* next;
} Node;

Node * createNode(struct sockaddr_in address, int sockfd) {
    Node * newNode = malloc(sizeof(Node));

    newNode->address = address;
    newNode->socket = sockfd;
    newNode->next = NULL;

    return newNode;
}

Node * push(Node * head, Node * newNode) {
    newNode->next = head;
    return newNode;
}

Node * getNode(Node * head, int index) {
    Node * current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

struct listening_socket {
    int sockfd;
};

struct listening_socket listeningfd;

void sig_handler(int sig) {
    printf("Closing connection...\n");
    if (sig == SIGINT || sig == SIGTERM) {
        close(listeningfd.sockfd);
    }
}

void die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int sockinit(struct sockaddr_in *address, socklen_t *sv_addr_len, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("socket error.");
    }
    bzero((char *) address, *sv_addr_len);
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) address, *sv_addr_len) == -1) {
        die("bind error.");
    }
    listeningfd.sockfd = sockfd;
    return sockfd;
}

int acceptclient(int sockfd, struct sockaddr_in *address, socklen_t *cl_addr_len) {
    bzero((char *) address, *cl_addr_len);
    int newsockfd = accept(sockfd, (struct sockaddr *) address, cl_addr_len);
    if (newsockfd == -1) {
        die("accept error.");
    }
    return newsockfd;
}

int clientHandler(int sockfd, struct sockaddr_in *address, socklen_t *cl_addr_len) {
    char *str = "Hello World!\n";
    char buff[128];
    unsigned long bytes;
    send(sockfd, str, strlen(str), 0);

    do {
        bzero(buff, sizeof(buff));
        bytes = (size_t)recv(sockfd, buff, sizeof(buff), 0);
        if (bytes > 0) {
            fprintf(stdout, "TERM: %s", buff);
        }
    } while(bytes > 0);
    close(sockfd);
    return 0;
}

Node *head = NULL;

int main(int argc, char *argv[]) {
    int listenfd, connfd;
    struct sockaddr_in sv_addr, cl_addr;
    socklen_t sv_addr_len;
    socklen_t cl_addr_len;
    char buffer[1024];
    pid_t pid;

    signal(SIGINT, sig_handler);

    sv_addr_len = sizeof(sv_addr);
    cl_addr_len = sizeof(cl_addr);

    listenfd = sockinit(&sv_addr, &sv_addr_len, PORT);

    listen(listenfd, 5);
    printf("Listening...\n");

    while (1) {
        // accept new client
        cl_addr_len = sizeof(cl_addr);
        connfd = acceptclient(listenfd, &cl_addr, &cl_addr_len);
        head = push(head, createNode(cl_addr, connfd));
        Node *curr = head;
        int i = 0;
        while (curr != NULL) {
            printf("%s\n", inet_ntoa(curr->address.sin_addr));
            curr = curr->next;
        }
        printf("%d clients connected\n", ++i);
        pid = fork();
        // CHILD STARTS HERE
        if (pid == 0) {
            close(listenfd);
            clientHandler(connfd, &cl_addr, &cl_addr_len);
            printf("I did my job.\n");
            close(connfd);
            return 0;
        }

        close(connfd);
        // CHILD ENDS HERE

        // PARENT LOOPS BACK

    }

}