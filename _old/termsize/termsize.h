#include <sys/ioctl.h>

int getTerminalSize(int *rows, int *cols) {
    struct winsize ws;
    int stdout_fd = fileno(stdout);
    // Use ioctl to get the terminal size
    if (ioctl(stdout_fd, TIOCGWINSZ, &ws) < 0) {
        perror("ioctl");
        *rows = -1;
        *cols = -1;
        return -1;
    } else {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    }
    return 0;
}