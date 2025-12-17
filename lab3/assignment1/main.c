#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SIZE 10 

void reverse(char *arr) {
    for (int i = 0; i < SIZE / 2; i++) {
        char temp = arr[i];
        arr[i] = arr[SIZE - 1 - i];
        arr[SIZE - 1 - i] = temp;
    }
}

int main() {
    int fd1 = creat("input.txt", 0644);
    char buffer[SIZE + 1];

    printf("Enter text (end with CTRL+D):\n");
    while (fgets(buffer, sizeof(buffer), stdin)) {
        int len = strlen(buffer);
  	buffer[strcspn(buffer, "\n")] = '\0'; 
        if (len < SIZE) {
            for (int i = len; i < SIZE; i++)
                buffer[i] = ' ';
            buffer[SIZE] = '\0';
        }

        write(fd1, buffer, SIZE);
    }

    close(fd1);
    
    int fd_in = open("input.txt", O_RDONLY);
    int fd_out = creat("output.txt", 0644);

    char rec[SIZE];

    while (read(fd_in, rec, SIZE) == SIZE) {

        reverse(rec);

        write(fd_out, rec, SIZE);
    }

    close(fd_in);
    close(fd_out);

    return 0;
}

