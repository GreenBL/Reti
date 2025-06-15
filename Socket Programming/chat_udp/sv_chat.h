#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define PORT 8080

typedef struct MSG {
    char sender[64];
    char content[2048];
} Message;
