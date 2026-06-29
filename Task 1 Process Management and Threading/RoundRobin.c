#include "common.h"

void simulate_round_robin() {
    int remaining[] = {10, 5, 8};
    int quantum = 4;
    printf("Round Robin Scheduler:\n");
    int done = 0;
    while (!done) {
        done = 1;
        for (int i = 0; i < 3; i++) {
            if (remaining[i] > 0) {
                done = 0;
                int ran = (remaining[i] > quantum) ? quantum : remaining[i];
                remaining[i] -= ran;
                printf("P%d ran %d units — %s\n", i+1, ran, 
                       remaining[i] == 0 ? "FINISHED" : "Running");
            }
        }
    }
}