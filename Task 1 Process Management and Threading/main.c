#include <stdio.h>
#include "common.h"

int shared_counter = 0;
pthread_mutex_t counter_mutex;
pthread_mutex_t resource_a;
pthread_mutex_t resource_b;
pthread_mutex_t resource_c; 

int main() {
    pthread_mutex_init(&counter_mutex, NULL);
    pthread_mutex_init(&resource_a, NULL);
    pthread_mutex_init(&resource_b, NULL);
    pthread_mutex_init(&resource_c, NULL);

    run_race_condition();
    run_mutex_fix();
    run_deadlocks();
    simulate_round_robin();

    pthread_mutex_destroy(&counter_mutex);
    pthread_mutex_destroy(&resource_a);
    pthread_mutex_destroy(&resource_b);
    pthread_mutex_destroy(&resource_c);
    return 0;
}