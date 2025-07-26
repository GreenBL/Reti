#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./analgram <string 1> <string 2>\n");
        perror("Too few args.");
        exit(1);
    }

    int control = 0;

    char *buff;


    int str1len = strlen(argv[1]);
    int str2len = strlen(argv[2]);

    char *str1 = malloc(str2len + 1);
    strcpy(str1, argv[1]);
    char *str2 = malloc(str1len + 1);
    strcpy(str2, argv[2]);

    if (str1 == NULL || str2 == NULL) {
        exit(1);
    }

    if (str1len > str2len || str2len > str1len) {
        printf("The strings are different.\n");
        return 0;
        exit(1);
    }

    buff = malloc(str2len + 1);
    strcpy(buff, str2);

    printf("buff waste check: %s\n", buff);

    for (int i = 0; i < str1len; ++i) {
        for (int j = 0; j < str2len; ++j) {
            if (buff[j] == '0') { // checks if the character has already been consumed
                    continue;
            }
            printf("[COMPARING %c to %c at index %d,%d\n", str1[i], str2[j], i, j);
            if (str1[i] == str2[j]) {
                printf("Found match for a %c\n", str1[i]);
                printf("SET 0 FOR %c at %d,%d", str2[j], i, j);
                buff[j] = '0'; // consume character
                break;
            }
        }
    }
    printf("buff waste check: %s\n", buff);
    for (int i = 0; i < str2len; ++i) {
        if (buff[i] == '0') {
            printf("%c ok!\n", str1[i]);
        } else {
            printf("The strings are different.\n");
            return 0;
        }
    }
    printf("The strings are anagrams of each other!.\n");

    free(str1);
    free(str2);
    free(buff);
    return 0;
}
