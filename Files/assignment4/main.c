#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main() {
    const char *filename = "log.txt";
    int fd;
    char input[BUF_SIZE];
    pid_t pid;
    ssize_t nread;

    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Errror opening");
        exit(1);
    }

    nread = read(0, input, BUF_SIZE - 1);
    if (nread < 0) {
        perror("read");
        close(fd);
        exit(1);
    }
    input[nread] = '\0';

    if (nread > 0 && input[nread - 1] == '\n') {
        input[nread - 1] = '\0';
    }

    pid = getpid();

    if (write(fd, "PID=", 4) < 0) {
        perror("write");
        close(fd);
        exit(1);
    }

    char pid_str[16];
    int pid_len = 0;
    int temp_pid = pid;
    char pid_buf[16];

    if (temp_pid == 0) {
        pid_buf[pid_len++] = '0';
    } else {
        while (temp_pid > 0) {
            pid_buf[pid_len++] = '0' + (temp_pid % 10);
            temp_pid /= 10;
        }
    }

    for (int i = pid_len - 1; i >= 0; i--) {
        pid_str[pid_len - 1 - i] = pid_buf[i];
    }
    pid_str[pid_len] = '\0';

    if (write(fd, pid_str, pid_len) < 0) {
        perror("write PID");
        close(fd);
        exit(1);
    }

    if (write(fd, ": ", 2) < 0) {
        perror("write");
        close(fd);
        exit(1);
    }

    size_t input_len = strlen(input);
    if (write(fd, input, input_len) < 0) {
        perror("write input");
        close(fd);
        exit(1);
    }

    if (write(fd, "\n", 1) < 0) {
        perror("write newline");
        close(fd);
        exit(1);
    }

    int offset = (int) lseek(fd, 0, SEEK_CUR);
    if (offset < 0) {
        perror("lseek");
        close(fd);
        exit(1);
    }
    printf("Final file offset after append: %d\n", offset);

    /* Explanation
    O_APPEND writes always go to the end, but the file offset still advances, so SEEK_CUR reflects the final offset correctly.
    */


    close(fd);
    return 0;
}

