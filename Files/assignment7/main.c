#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

int main()
{
    char file1[256];
    char file2[256];
    int fd1;
    int fd2;
    unsigned char buf1[BUFF_SIZE];
    unsigned char buf2[BUFF_SIZE];
    ssize_t n1;
    ssize_t n2;
    long byte_index = 0;

    printf("Enter first file path: ");
    scanf("%255s", file1);
    printf("Enter second file path: ");
    scanf("%255s", file2);

    fd1 = open(file1, O_RDONLY);
    if (fd1 < 0)
    {
        perror("open file1");
        exit(1);
    }

    fd2 = open(file2, O_RDONLY);
    if (fd2 < 0)
    {
        perror("open file2");
        close(fd1);
        exit(1);
    }

    while (1)
    {
        n1 = read(fd1, buf1, BUFF_SIZE);
        if (n1 < 0)
        {
            perror("read file1");
            close(fd1);
            close(fd2);
            exit(1);
        }

        n2 = read(fd2, buf2, BUFF_SIZE);
        if (n2 < 0)
        {
            perror("read file2");
            close(fd1);
            close(fd2);
            exit(1);
        }

        if (n1 == 0 && n2 == 0)
        {
            break;
        }

        ssize_t min_bytes = (n1 < n2) ? n1 : n2;
        for (ssize_t i = 0; i < min_bytes; i++)
        {
            if (buf1[i] != buf2[i])
            {
                printf("Files differ at byte %ld\n", byte_index + i);
                close(fd1);
                close(fd2);
                exit(1);
            }
        }

        if (n1 != n2)
        {
            printf("Files differ at byte %ld\n", byte_index + min_bytes);
            close(fd1);
            close(fd2);
            exit(1);
        }

        byte_index += min_bytes;
    }

    printf("Files are identical\n");
    close(fd1);
    close(fd2);
    return 0;
}

