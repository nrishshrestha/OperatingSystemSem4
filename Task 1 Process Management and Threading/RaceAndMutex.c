#include "common.h"

// assigning core task to the threads
void* increment_task(void* arg) {
    int use_mutex = *(int*)arg;
    for (int i = 0; i < 100000; i++) {
        if(use_mutex) {
            pthread_mutex_lock(&counter_mutex);
            shared_counter++;
            pthread_mutex_unlock(&counter_mutex);
        } else {
            shared_counter++; // race condition occurs here 
        }
    }
    return NULL;
}

void run_race_condition() {
    pthread_t t1, t2, t3;
    int use_mutex = 0; // 0 means Race Condition
    shared_counter = 0; 
    
    printf("RACE CONDITION:\n");
    pthread_create(&t1, NULL, increment_task, &use_mutex);
    pthread_create(&t2, NULL, increment_task, &use_mutex);
    pthread_create(&t3, NULL, increment_task, &use_mutex);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Expected: 300000, Actual: %d (data loss)\n", shared_counter);
}

void run_mutex_fix() {
    pthread_t t1, t2, t3;
    int use_mutex = 1; // 1 means Mutex Fix
    shared_counter = 0; 
    
    printf("MUTEX FIX\n");
    pthread_create(&t1, NULL, increment_task, &use_mutex);
    pthread_create(&t2, NULL, increment_task, &use_mutex);
    pthread_create(&t3, NULL, increment_task, &use_mutex);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Expected: 300000, Actual: %d (no data loss)\n", shared_counter);
}