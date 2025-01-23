#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char** argv) {

    // Dichiarazione di un puntatore a file
    FILE *file;

    int character = 0x0;
    file = fopen("file.txt", "rb");
    if(file == NULL) {
        printf("\nNULL file Error!\n");
        return -1;
    }
    int index = 0;
    char buffer[1024];
    while ((character = fgetc(file)) != EOF) {
        // Process each character
        putchar(toupper(character));
        fflush(stdout);
        usleep(100000);
        if(character == 'o') {
            usleep(10000);
        }
    }
    puts("");
    fclose(file);

    return 0;
}