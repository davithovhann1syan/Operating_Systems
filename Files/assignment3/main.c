#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


int main() {
    char path[250];
    int fd;
    int total_size;
    unsigned char byte;

    printf("Enter path to an text file: ");
    scanf("%249s", path);

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the file");
        exit(1);
    }

    total_size = lseek(fd, 0, SEEK_END);
    if (total_size < 0) {
        perror("Error lseek");
        close(fd);
        exit(1);
    }

    if (total_size == 0) {
        printf("\n");
        close(fd);
        exit(0);
    }

    for (int position = total_size - 1; position >= 0; position--) {

        if (lseek(fd, position, SEEK_SET) < 0) {
            perror("Error lseek");
            close(fd);
            exit(1);
        }

        ssize_t r = read(fd, &byte, 1);
        if (r < 0) {
            perror("Error reading");
            close(fd);
            exit(1);
        }

        int ret = write(STDOUT_FILENO, &byte, 1);
        if (ret < 0) {
            perror("Error writing");
            close(fd);
            exit(1);
        }
        
        if (position == 0) break;
    }
    
    write(STDOUT_FILENO, "\n", 1);

    close(fd);
    return 0;
}

