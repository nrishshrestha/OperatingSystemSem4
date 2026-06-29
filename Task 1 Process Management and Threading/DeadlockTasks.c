#include "common.h"

// Deadlock Demonstration using Three Threads
void* deadlock_t1(void* arg) {
    pthread_mutex_lock(&resource_a);
    printf("Thread1: got A, waiting for B.\n");
    sleep(1); 
    if (pthread_mutex_trylock(&resource_b) != 0)
        printf("Thread1: DEADLOCK DETECTED — B is held by Thread2\n");
    else
        pthread_mutex_unlock(&resource_b);
    pthread_mutex_unlock(&resource_a);
    return NULL;
}

void* deadlock_t2(void* arg) {
    pthread_mutex_lock(&resource_b);
    printf("Thread2: got B, waiting for C.\n");
    sleep(1); 
    if (pthread_mutex_trylock(&resource_c) != 0)
        printf("Thread2: DEADLOCK DETECTED — C is held by Thread3\n");
    else
        pthread_mutex_unlock(&resource_c);
    pthread_mutex_unlock(&resource_b);
    return NULL;
}

void* deadlock_t3(void* arg) {
    pthread_mutex_lock(&resource_c);
    printf("Thread3: got C, waiting for A.\n");
    sleep(1); 
    if (pthread_mutex_trylock(&resource_a) != 0)
        printf("Thread3: DEADLOCK DETECTED — A is held by Thread1\n");
    else
        pthread_mutex_unlock(&resource_a);
    pthread_mutex_unlock(&resource_c);
    return NULL;
}

// Deadlock prevention
// currently all threads follow the same order: Lock A -> B -> C
void* safe_task(void* arg) {
    pthread_mutex_lock(&resource_a);
    pthread_mutex_lock(&resource_b);
    pthread_mutex_lock(&resource_c);
    printf("All threads A, B, and C acquired safely.\n");
    pthread_mutex_unlock(&resource_c);
    pthread_mutex_unlock(&resource_b);
    pthread_mutex_unlock(&resource_a);
    return NULL;
}

void run_deadlocks() {
    pthread_t t1, t2, t3;

    printf("\nDeadlock Demonstration:\n");
    pthread_create(&t1, NULL, deadlock_t1, NULL);
    pthread_create(&t2, NULL, deadlock_t2, NULL);
    pthread_create(&t3, NULL, deadlock_t3, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Deadlock Prevention Demonstration\n");
    pthread_create(&t1, NULL, safe_task, NULL);
    pthread_create(&t2, NULL, safe_task, NULL);
    pthread_create(&t3, NULL, safe_task, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}