#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 1000000
#define NUM_THREADS 4

int data[N];
int hist[10];

pthread_spinlock_t slock;
pthread_mutex_t mlock;
sem_t sem;

int mode = 0;

void reset_hist() {
    for (int i = 0; i < 10; i++)
        hist[i] = 0;
}

void* worker(void *arg) {
    int id = (int)(long)arg;
    int chunk = N / NUM_THREADS;
    int start = id * chunk;
    int end = start + chunk;

    for (int i = start; i < end; i++) {
        int d = data[i];

        if (mode == 1) {
            pthread_spin_lock(&slock);
            hist[d]++;
            pthread_spin_unlock(&slock);
        }
        else if (mode == 2) {
            pthread_mutex_lock(&mlock);
            hist[d]++;
            pthread_mutex_unlock(&mlock);
        }
        else if (mode == 3) {
            sem_wait(&sem);
            hist[d]++;
            sem_post(&sem);
        }
        else {
            hist[d]++;
        }
    }

    return NULL;
}

void run_test(const char *mode_name, int m) {
    mode = m;
    reset_hist();

    printf("Mode %d (%s)\n", m, mode_name);

    pthread_t t[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&t[i], NULL, worker, (void*)(long)i);

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL);

    for (int i = 0; i < 10; i++)
        printf("hist[%d] = %d (expected 100000)\n", i, hist[i]);
    printf("--------------------------------------------------------\n");
}

int main() {

    for (int i = 0; i < N; i++)
        data[i] = i % 10;

    pthread_spin_init(&slock, PTHREAD_PROCESS_PRIVATE);
    pthread_mutex_init(&mlock, NULL);
    sem_init(&sem, 0, 1);

    run_test("No Sync", 0);
    run_test("Spin Lock", 1);
    run_test("Mutex", 2);
    run_test("Semaphore", 3);

    pthread_spin_destroy(&slock);
    pthread_mutex_destroy(&mlock);
    sem_destroy(&sem);

    return 0;
}

