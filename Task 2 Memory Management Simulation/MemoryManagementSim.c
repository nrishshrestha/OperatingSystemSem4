#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 1024
#define PAGE_SIZE 256
#define FRAMES 3

int frames[FRAMES];
int page_faults = 0;
int hits = 0;

void init_frames();
bool is_hit(int page);
void run_fifo(int pages[], int n);
void run_lru(int pages[], int n);

// main function
int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5};
    int n = 7;
    printf("Configurable Page Size: %d bytes\n", PAGE_SIZE);
    
    // running fifo
    run_fifo(pages, n);
    printf("FIFO Faults: %d, Hit Ratio: %.2f\n", page_faults, (float)hits/(hits+page_faults));
    
    // running LRU
    run_lru(pages, n);
    printf("LRU Faults: %d, Hit Ratio: %.2f\n", page_faults, (float)hits/(hits+page_faults));
    
    return 0;
}

// Initializing empty memry frames (-1)
void init_frames() {
    for(int i = 0; i < FRAMES; i++) frames[i] = -1;
}

bool is_hit(int page) {
    for(int i = 0; i < FRAMES; i++) if(frames[i] == page) return true;
    return false;
}

// First In First Out
void run_fifo(int pages[], int n) {
    init_frames();
    int next_replace = 0;
    page_faults = 0; hits = 0;
    for(int i = 0; i < n; i++) {
        if(is_hit(pages[i])) { 
            hits++; 
            printf("Page %d: Hit!  | Frames: [%d, %d, %d]\n", pages[i], frames[0], frames[1], frames[2]); //Tracking page hits or misses
        } else {
            frames[next_replace] = pages[i];
            next_replace = (next_replace + 1) % FRAMES;
            page_faults++;
            printf("Page %d: Fault!| Frames: [%d, %d, %d]\n", pages[i], frames[0], frames[1], frames[2]); //Tracking page hits or misses
        }
    }
}

// Least Recently unsigned
void run_lru(int pages[], int n) {
    init_frames();
    int time[FRAMES] = {0}; // helps track usage
    page_faults = 0; hits = 0;
    for(int i = 0; i < n; i++) {
        if(is_hit(pages[i])) { 
            hits++; 
            printf("Page %d: Hit!  | Frames: [%d, %d, %d]\n", pages[i], frames[0], frames[1], frames[2]); //Tracking page hits or misses
            // NEW ADDED: Simple LRU logic - find the index with lowest 'time'
            int lru_idx = 0;
            for(int j = 1; j < FRAMES; j++) if(time[j] < time[lru_idx]) lru_idx = j;
            
            frames[lru_idx] = pages[i];
            time[lru_idx] = i; // update time to current step
            page_faults++;
            printf("Page %d: Fault!| Frames: [%d, %d, %d]\n", pages[i], frames[0], frames[1], frames[2]); //Tracking page hits or misses
        }
    }
}