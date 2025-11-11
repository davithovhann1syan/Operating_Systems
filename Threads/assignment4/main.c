#include <stdio.h>
#include <pthread.h>

void* square(void* arg) {
    int num = *(int*)arg;
    printf("Square of %d is %d\n", num, num * num);
    return NULL;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;

    pthread_t threads[size];

    for (int i = 0; i < size; i++) {
        pthread_create(&threads[i], NULL, square, &arr[i]);
    }

    for (int i = 0; i < size; i++) {
        if (pthread_join(threads[i], NULL)!= 0) {
            perror("Failed to join thread 2");
            return 1;
        }
    }

    return 0;
}

