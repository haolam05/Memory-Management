// File Purpose	: implementation of function testbenches for memory manager

// INCLUDES LIBRARIES
#include <stdlib.h>
#include <time.h>
#include <string.h>

// INCLUDES FILES
#include "mem.h"

// DEFINE CONSTANTS
// maximum numbers of allowed parameters
#ifndef MAX_PARAMS
#define MAX_PARAMS 6
#endif
// print out parameters if debug mode is on, default is off
#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

// DECLARE FUNCTIONS
void set0x(void *curr_ptr, int set);
void print_stats(int countTrials, int ntrials, clock_t start,
                 uintptr_t *total_size, uintptr_t *total_free,
                 uintptr_t *n_free_blocks);
int get_rand(int pct);
int set_params(char *param_name, int argc, char **argv, int def_val, int i);
int run_a_trial(int pctget, void *ptr_addrs[], int length,
                int small_limit, int large_limit, int pctlarge);

int main(int argc, char **argv) {
    // check number of arguments
    if (argc > MAX_PARAMS + 1) {
        printf("Argument Error: %d, Too many arguments!!!\n", argc - 1);
        return EXIT_FAILURE;
    }
    int i = 1;

    // total number of getmem plus freemem calls to randomly perform,
    // during this test. Default 10000.
    int ntrials     = set_params("ntrails", argc, argv, 10000, i++);

    // percent of the total getmem/freemem calls that should be getmem.
    // Default 50.
    int pctget      = set_params("pctget", argc, argv, 50, i++);

    // percent of the getmem calls that should request "large" blocks
    // with a size greater than small_limit. Default 10.
    int pctlarge    = set_params("pctlarge", argc, argv, 10, i++);

    // largest size in bytes of a "small" block. Default 200.
    int small_limit = set_params("small_limit", argc, argv, 200, i++);

    // largest size in bytes of a "large" block. Default 20000.
    int large_limit = set_params("large_limit", argc, argv, 20000, i++);

    // seed to generate random numbers.
    int random_seed = set_params("random_seed", argc, argv, rand(), i++);
    srand(random_seed);

    // set up statistic parameters for memory manager
    void *ptr_addrs[ntrials];
    int length = 0;
    int countTrials = 1;
    uintptr_t *total_size    = malloc(sizeof(uintptr_t));
    uintptr_t *total_free    = malloc(sizeof(uintptr_t));
    uintptr_t *n_free_blocks = malloc(sizeof(uintptr_t));
    clock_t start;

    start = clock();
    while (countTrials < ntrials) {
        length += run_a_trial(pctget, ptr_addrs, length, small_limit,
                              large_limit, pctlarge);
        countTrials++;

        if (countTrials % (ntrials / 10) == 0) {  // every 10%
            get_mem_stats(total_size, total_free, n_free_blocks);
            print_stats(countTrials, ntrials, start, total_size,
                        total_free, n_free_blocks);
        }
    }

    // free statistics parameters pointers
    free(total_size);
    free(total_free);
    free(n_free_blocks);

    // print out final statistics in "mem_stats.txt" file
    FILE *f = fopen("mem_stats.txt", "w");
    print_heap(f);
    fclose(f);

    return EXIT_SUCCESS;
}

// DO:
//  1) run a trial of getmem/freemem call
//  2) returns whether length of ptr_addrs is increment, decrement, or unchanged
//  int pctget      : the percent of getmem calls
//  int *ptr_addrs[]: the array of pointers to the allocated memory
//  int length      : the length of the array
//  int small_limit : the largest size in bytes of a "small" block
//  int large_limit : the largest size in bytes of a "large" block
//  int pctlarge    : the percent of getmem calls that request "large" blocks
int run_a_trial(int pctget, void *ptr_addrs[], int length,
                int small_limit, int large_limit, int pctlarge) {
    int getMem;
    int largeBlock;
    uintptr_t blockSize;
    int incr_length = 0;

    getMem = get_rand(pctget);
    if (getMem) {  // getmem
        blockSize = (rand() % small_limit) + 1;  // [1, small_limit]
        largeBlock = get_rand(pctlarge);
        if (largeBlock) {
            while (blockSize <= small_limit) {   // [small, large_limit]
                blockSize = (rand() % large_limit) + 1;
            }
        }

        // if requested block size < MIN_BLOCK_SIZE bytes, give default 0xFE
        void *curr_ptr = getmem(blockSize < MIN_BLOCK_SIZE ? (uintptr_t) 0xFE :
                                                             blockSize);

        if (curr_ptr != NULL) {
            set0x(curr_ptr, 1);
            ptr_addrs[length] = curr_ptr;
            incr_length++;
        }
    } else {  // freemem
        if (length > 0) {
            int block_idx = rand() % length;
            set0x(ptr_addrs[block_idx], 0);
            freemem(ptr_addrs[block_idx]);
            ptr_addrs[block_idx] = ptr_addrs[length - 1];
            ptr_addrs[length - 1] = NULL;
            incr_length--;
        }
    }

    return incr_length;
}

// DO:
//  1) stores value(0xFE-set) or (0x00-free) in each of the first
//      MIN_BLOCK_SIZE bytes of the allocated block
// Parameters:
//  void *curr_ptr : pointer to start of block that wanted to be set
//  int set        : set(1) or free(0)
void set0x(void *curr_ptr, int set) {
    unsigned char hexVal = set ? 0xFE : 0x00;
    for (int i = 0; i < MIN_BLOCK_SIZE; i++) {
        *((unsigned char *)((uintptr_t)curr_ptr + i)) = hexVal;
    }
}

// DO:
//  1) prints the statistics of the memory manager
// Parameters:
//  int countTrials          : the number of trials
//  int ntrials              : the total number of trials
//  clock_t start            : the start time
//  uintptr_t *total_size    : the total size of the heap
//  uintptr_t *total_free    : the total free space in the heap
//  uintptr_t *n_free_blocks : the number of free blocks in the heap
void print_stats(int countTrials, int ntrials, clock_t start,
                 uintptr_t *total_size, uintptr_t *total_free,
                 uintptr_t *n_free_blocks) {
    printf("%d%% complete.\n", countTrials * 100 / ntrials);
    printf("\tTotal bytes acquired from system: %lu bytes.\n", *total_size);
    printf("\tTotal free bytes: %lu bytes.\n", *total_free);
    printf("\tTotal free blocks: %lu blocks.\n", *n_free_blocks);

    // average number of bytes per free block
    if (*n_free_blocks > 0) {
        printf("\tAverage bytes per free block: %lu bytes.\n",
               *total_free / *n_free_blocks);
    }

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\tTime elapsed: %f seconds\n\n", cpu_time_used);
}

// DO:
//  1) generate random number based on given pct
// Parameters:
//  int pct: the percentage of the total number of trials
int get_rand(int pct) {
    return rand() % 100 < pct;
}

// DO:
//  1) set value for a parameter
//  2) if the random_seed parameter is specified,
//     generate a coin flip value: 0 or 1
//  3) return the value for the parameter
// Parameters:
//    char *param_name  : name of the parameter
//    int argc          : number of arguments from command line
//    char **argv       : arguments values from command line
//    int def_val       : default parameter value
//    int i             : index of the parameter in argv
//    int is_rand_param : 1 if the parameter is a random number
int set_params(char* param_name, int argc, char** argv, int def_val, int i) {
    int chosen_val;
    chosen_val = i < argc ? atoi(argv[i]) : def_val;

    if (DEBUG_MODE) {
        printf("%s = %d\n", param_name, chosen_val);
    }

    return chosen_val;
}
