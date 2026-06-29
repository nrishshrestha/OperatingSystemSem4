#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// declaring global variables
extern int shared_counter;
extern pthread_mutex_t counter_mutex;
extern pthread_mutex_t resource_a;
extern pthread_mutex_t resource_b;
extern pthread_mutex_t resource_c;

// prototypes for functions
void* increment_task(void* arg);
void run_race_condition();
void run_mutex_fix();
void run_deadlocks();
void simulate_round_robin();

#endif