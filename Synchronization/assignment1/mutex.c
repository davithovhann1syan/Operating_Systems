#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define NUM_INCREMENTS 1000000

long long counter = 0;
pthread_mutex_t mutex;

void* increment_counter(void* argument) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    printf("Expected value: %d * %d = %lld\n", 
           NUM_THREADS, NUM_INCREMENTS, 
           (long long)NUM_THREADS * NUM_INCREMENTS);


    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
            perror("pthread_create failed");
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            pthread_mutex_destroy(&mutex);
            return 1;
        }
    }
    
    printf("Final counter value: %lld\n", counter);
    
    return 0;
}
