#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 4096

int main() {
    const char *filename = "numbers.txt";
    int fd;
    char buffer[BUFF_SIZE];
    ssize_t nread;
    int line_count = 0;
    int offset = 0;

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) exit(1);

    for (int i = 1; i <= 10; i++) {
        char num[16];
        int len = snprintf(num, sizeof(num), "%d\n", i);
        if (write(fd, num, len) < 0) exit(1);
    }

    close(fd);

    fd = open(filename, O_RDWR);
    if (fd < 0) exit(1);

    int line_start = 0;
    while ((nread = read(fd, buffer, BUFF_SIZE)) > 0) {
        for (ssize_t i = 0; i < nread; i++) {
            if (line_count == 3) break;
            if (buffer[i] == '\n') line_count++;
            line_start++;
        }
        if (line_count == 3) break;
    }

    if (nread < 0) {
        exit(1);
    }

    if (lseek(fd, line_start, SEEK_SET) < 0) {
        exit(1);
    }

    off_t remainder_size = lseek(fd, 0, SEEK_END) - line_start;
    if (remainder_size < 0) {
        exit(1);
    }
    char *remainder = malloc(remainder_size);
    if (!remainder) {
        exit(1);
    }

    if (lseek(fd, line_start, SEEK_SET) < 0) {
        exit(1);
    }
    if (read(fd, remainder, remainder_size) != remainder_size) {
        exit(1);
    }

    if (lseek(fd, line_start, SEEK_SET) < 0) {
        exit(1);
    }
    const char *new_line = "100\n";
    if (write(fd, new_line, strlen(new_line)) != (ssize_t)strlen(new_line)) {
        exit(1);
    }
    if (write(fd, remainder, remainder_size) != remainder_size) {
        exit(1);
    }

    free(remainder);
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("failed to open");
        exit(1);
    }

    while ((nread = read(fd, buffer, BUFF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buffer, nread) < 0) {
            exit(1);
        }
    }

    close(fd);
    return 0;
}


