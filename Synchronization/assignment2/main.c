#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_DEPOSIT_THREADS 4
#define NUM_WITHDRAW_THREADS 4
#define NUM_OPERATIONS 100000

long long balance = 0;
pthread_mutex_t mutex;
pthread_spinlock_t spinlock;

int use_mutex = 1;
int long_cs = 0;

void simulate_work() {
    if (long_cs) {
        long long sum = 0;
        for (int i = 0; i < 50000; i++) {
            sum += i;
        }
    }
}

void* deposit_thread(void* arg) {
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        if (use_mutex) {
            pthread_mutex_lock(&mutex);
            balance++;
            simulate_work();
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_spin_lock(&spinlock);
            balance++;
            simulate_work();
            pthread_spin_unlock(&spinlock);
        }
    }
    return NULL;
}

void* withdraw_thread(void* arg) {
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        if (use_mutex) {
            pthread_mutex_lock(&mutex);
            balance--;
            simulate_work();
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_spin_lock(&spinlock);
            balance--;
            simulate_work();
            pthread_spin_unlock(&spinlock);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <mutex|spin> <short|long>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "mutex") == 0) {
        use_mutex = 1;
    } else if (strcmp(argv[1], "spin") == 0) {
        use_mutex = 0;
    } else {
        printf("Error: First argument must be 'mutex' or 'spin'\n");
        return 1;
    }

    if (strcmp(argv[2], "short") == 0) {
        long_cs = 0;
    } else if (strcmp(argv[2], "long") == 0) {
        long_cs = 1;
    } else {
        printf("Error: Second argument must be 'short' or 'long'\n");
        return 1;
    }

    if (use_mutex) {
        pthread_mutex_init(&mutex, NULL);
    } else {
        pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    }

    pthread_t deposit_threads[NUM_DEPOSIT_THREADS];
    pthread_t withdraw_threads[NUM_WITHDRAW_THREADS];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_DEPOSIT_THREADS; i++) {
        pthread_create(&deposit_threads[i], NULL, deposit_thread, NULL);
    }
    for (int i = 0; i < NUM_WITHDRAW_THREADS; i++) {
        pthread_create(&withdraw_threads[i], NULL, withdraw_thread, NULL);
    }

    for (int i = 0; i < NUM_DEPOSIT_THREADS; i++) {
        pthread_join(deposit_threads[i], NULL);
    }
    for (int i = 0; i < NUM_WITHDRAW_THREADS; i++) {
        pthread_join(withdraw_threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    printf("Expected: 0, Result: %lld, Time: %.6f\n", balance, time_taken);

    if (use_mutex) {
        pthread_mutex_destroy(&mutex);
    } else {
        pthread_spin_destroy(&spinlock);
    }

    return 0;
}
