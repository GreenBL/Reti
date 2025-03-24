#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// sys/types.h e sys/socket.h
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080

typedef struct Message {
    char* USER[32];
    char* MESSAGE[128];
} MSG_USER;

void prompt_username(char instance_username[]) {
    char temp_buffer[256]; // general use buffer
    int size = 0;
    while (size == 0) {
        printf("Welcome, insert username to continue (max 32 characters): ");
        size = strlen(fgets(temp_buffer, sizeof(temp_buffer), stdin));
        if (size > 33) {
            printf("ERROR : Name is too long!\n");
            size = 0;
        } else {
            strcpy(instance_username, temp_buffer);
            instance_username[strcspn(instance_username, "\n")] = 0;
            printf("Welcome, %s!\n", instance_username);
        }
    }
}

int main (int argc, char argv[]) {

    char instance_username[33]; // username buffer
    prompt_username(instance_username);

    printf("Congratulations %s! You are now connecting...\n", instance_username);


}
