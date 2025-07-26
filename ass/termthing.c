#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void printbar(int mode) {
    for (int i = 0; i < 100; ++i) {
        printf("█");
        if (i == 99 && mode == 1) {
            printf("\n");
        }
    }
}

int main() {

    char buff[BUFSIZ];
    size_t bufflen = sizeof(buff);
    system("clear");
    printbar(1);
    fprintf(stdout, "\n\t\t\tBENVENUTI NEL SISTEMA ROMANO\t\t\t\n");
    for (int i = 0; i < 18; ++i) {
        printf("\n");
    }
    printbar(0);
    fflush(stdout);
    getline(buff, &bufflen, stdin);

    return 0;
}
