#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define TOTAL_ITEMS 20

int buffer[BUFFER_SIZE];
int in_pos = 0;
int out_pos = 0;

sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t buffer_mutex;

int items_produced = 0;
int items_consumed = 0;

void* producer(void* arg) {
    for (int i = 0; i < TOTAL_ITEMS / NUM_PRODUCERS; i++) {
        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);
        
        buffer[in_pos] = ++items_produced;
        in_pos = (in_pos + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < TOTAL_ITEMS / NUM_CONSUMERS; i++) {
        sem_wait(&full_slots);
        pthread_mutex_lock(&buffer_mutex);
        
        buffer[out_pos];
        out_pos = (out_pos + 1) % BUFFER_SIZE;
        items_consumed++;
        
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_slots);
    }
    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);
    
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }
    
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    printf("Produced: %d, Consumed: %d\n", items_produced, items_consumed);
    
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&buffer_mutex);
    
    return 0;
}
