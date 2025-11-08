#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define ONE_MB (1024*1024)

int main() {
    const char *filename = "sparse.bin";
    int fd;
    int size;

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    if (write(fd, "START", 5) < 0) {
        perror("write START");
        close(fd);
        exit(1);
    }

    if (lseek(fd, ONE_MB, SEEK_CUR) < 0) {
        perror("lseek");
        close(fd);
        exit(1);
    }

    if (write(fd, "END", 3) < 0) {
        perror("write END");
        close(fd);
        exit(1);
    }

    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open read");
        exit(1);
    }

    size = (int) lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek end");
        close(fd);
        exit(1);
    }

    printf("Apparent file size: %d bytes\n", size);

    close(fd);

    /* Disk usage (du) shows much smaller size because the file is sparse: 
       the unwritten region is represented by metadata and not actually stored on disk. */

    return 0;
}

