#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {

    // Dichiarazione di un puntatore a file
    FILE *file;

    int character = 0x0;
    file = fopen("file.txt", "r");
    if(file == NULL) {
        printf("\nNULL file Error!\n");
        return -1;
    }
    int index = 0;
    char buffer[1024];
    while ((character = fgetc(file)) != EOF) {
        // Process each character
        putchar(character);
    }
    puts("");
    fclose(file);

    return 0;
}