#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* print_message(void* arg) {
    printf("Thread with TID: %lu is running\n", pthread_self());
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
        usleep(100000); // 100 ms
    }

    printf("\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;

    if (pthread_create(&thread1, NULL, print_message, NULL) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_create(&thread2, NULL, print_message, NULL) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    if (pthread_create(&thread3, NULL, print_message, NULL) != 0) {
        perror("Failed to create thread 3");
        return 1;
    }
    if (pthread_join(thread1, NULL) != 0) {
        perror("Failed to join thread 1");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("Failed to join thread 2");
        return 1;
    }

    if (pthread_join(thread3, NULL) != 0) {
        perror("Failed to join thread 3");
        return 1;
    }

    printf("All threads have finished execution\n");

    return 0;
}

