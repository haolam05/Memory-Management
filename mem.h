#ifndef MEM_H_
#define MEM_H_

// File Purpose	: header file containing declarations of the public functions in the memory manager

// INCLUDES LIBRARIES
#include <stdio.h>
#include <inttypes.h>

// DEFINE CONSTANTS
// minimumm block size
#ifndef MIN_BLOCK_SIZE
#define MIN_BLOCK_SIZE 16
#endif

// return a *void pointer to a memory block of requested size
void *getmem(uintptr_t size);

// put the memory block at *p back into the free list
void freemem(void *p);

// compute the statistics of the memory manager, and store them in the
// 3 variables passed in as arguments
void get_mem_stats(uintptr_t *total_size,
                   uintptr_t *total_free, uintptr_t *n_free_blocks);

// print the current state of the freelist
void print_heap(FILE *f);

#endif  // MEM_H_
