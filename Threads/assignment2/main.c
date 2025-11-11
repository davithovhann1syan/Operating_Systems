#include <stdio.h>
#include <pthread.h>

int arr[] = {2, 4, 6, 8, 10, 12};
int size = 6;

void* sum_half(void* arg) {
    int start = *(int*)arg;
    int end = start + size/2;
    int sum = 0;

    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    printf("Thread with TID: %lu sum = %d\n", pthread_self(), sum);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    int start1 = 0;          
    int start2 = size / 2;    

    pthread_create(&t1, NULL, sum_half, &start1);
    pthread_create(&t2, NULL, sum_half, &start2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

