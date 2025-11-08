#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(void) {

    char source_path[250];
    char dest_path[250];
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];
    long total_bytes = 0;

    printf("Please enter source path: ");
    scanf("%249s", source_path);
    printf("Please enter destination path: ");
    scanf("%249s", dest_path);

    errno = 0;

    int fd_source = open(source_path, O_RDONLY);

    if (fd_source < 0) {
        perror("Error opening source file");
        exit(1);
    }

    int fd_destination = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_destination < 0) {
        perror("Error opening or creating destination file");
        close(fd_source);
        exit(1);
    }

    while ((bytes_read = read(fd_source, buffer, BUFFER_SIZE)) > 0) {

        ssize_t bytes_written = write(fd_destination, buffer, bytes_read);

        if (bytes_written < 0) {
            perror("Error writing to destination file");
            close(fd_source);
            close(fd_destination);
            return 1;
        }

        total_bytes += bytes_written;
    }

    if (bytes_read < 0) {
        perror("Error reading from source file");
    }

    close(fd_source);
    close(fd_destination);

    printf("Copy complete: %ld bytes copied.\n", total_bytes);


    return 0;
}

