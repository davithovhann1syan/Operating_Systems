#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
int main() {
    const char *filename = "data.txt";
    const char *text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // 26 bytes

    int fd;
    ssize_t bytes_written;
    int total_size;
    char buffer[BUFFER_SIZE];

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error while opening or creating file");
        exit(1);
    }

    bytes_written = write(fd, text, 26);
    if (bytes_written < 0) {
        perror("Error writing text");
        close(fd);
        exit(1);
    }

    close(fd);

    fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("Error while opening or creating file");
        exit(1);
    }

    total_size = lseek(fd, 0, SEEK_END);
    if (total_size < 0) {
        perror("Error while opening or creating file");
        close(fd);
        exit(1);
    }
    printf("Initial size: %d bytes\n", total_size);

    if (ftruncate(fd, 10) < 0) {
        perror("failed to truncate file");
        close(fd);
        exit(1);
    }

    total_size = lseek(fd, 0, SEEK_END);
    if (total_size < 0) {
        perror("failed lseek");
        close(fd);
        exit(1);
    }
    printf("Size after truncation: %d bytes\n", total_size);

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("failed to lseek");
        close(fd);
        exit(1);
    }

    ssize_t end = read(fd, buffer, sizeof(buffer) - 1);
    if (end < 0) {
        perror("Error reading the file");
        close(fd);
        exit(1);
    }

    buffer[end] = '\0';
    printf("Remaining content: \"%s\"", buffer);
    printf("\n");

    close(fd);
    return 0;
}

