#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define K 3
#define N 10

sem_t printers;
int printers_in_use = 0;
pthread_mutex_t counter_mutex;

void* print_job(void* arg) {
    int id = *(int*)arg;
    
    sem_wait(&printers);
    
    pthread_mutex_lock(&counter_mutex);
    printers_in_use++;
    printf("Thread %d started printing. Printers in use: %d\n", id, printers_in_use);
    pthread_mutex_unlock(&counter_mutex);
    
    usleep(500000);
    
    pthread_mutex_lock(&counter_mutex);
    printers_in_use--;
    printf("Thread %d finished printing. Printers in use: %d\n", id, printers_in_use);
    pthread_mutex_unlock(&counter_mutex);
    
    sem_post(&printers);
    
    return NULL;
}

int main() {
    pthread_t threads[N];
    int thread_ids[N];
    
    sem_init(&printers, 0, K);
    pthread_mutex_init(&counter_mutex, NULL);
    
    for (int i = 0; i < N; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, print_job, &thread_ids[i]);
    }
    
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&printers);
    pthread_mutex_destroy(&counter_mutex);
    
    return 0;
}
