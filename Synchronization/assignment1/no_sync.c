#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM_INCREMENTS 1000000

long long counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    printf("Expected value: %d * %d = %lld\n", 
           NUM_THREADS, NUM_INCREMENTS, 
           (long long)NUM_THREADS * NUM_INCREMENTS);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }
    
    printf("Final counter value: %lld\n", counter);
    
    return 0;
}