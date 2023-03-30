#ifndef MEM_IMPL_H_
#define MEM_IMPL_H_

// File Purpose	: header file with declarations of internal implementation details

// INCLUDES LIBRARIES
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

// DEFINE CONSTANTS
// offset header size (base 16)
#ifndef HEADER_SIZE
#define HEADER_SIZE 16
#endif
// allocated block size can NOT be greater than
// 20% the requested block size
#ifndef MAX_LARGER_PCT
#define MAX_LARGER_PCT 1.2
#endif
// default size of memory block to be allocated
#ifndef DEFAULT_MALLOC_SIZE
#define DEFAULT_MALLOC_SIZE pow(16, 4)
#endif

// free node constructors
typedef struct free_node {
    uintptr_t size;          // size of the free block
    struct free_node* next;  // pointer to the next free block
} free_node;

// ask underlying system for more memory
struct free_node *newMemBlock(uintptr_t size);

// implementation of check_heap
void check_heap();

// initialize freelist header in global scope. Default is NULL
struct free_node *freelist;

// to keep track of total bytes allocated by underlying system
uintptr_t total_bytes_allocated;

// return a pointer to a block of requested size on freelist
void * findBlock(uintptr_t size, struct free_node *curr,
                                 struct free_node *prev);

// put a given new block on freelist (in sorted order)
void putNewBlockOnFreelist(struct free_node *newBlock);

// start from freelist head, combine adjacent blocks that found
void combine_if_found_adjacents();

// check if 2 blocks are adjacent, regardless of their order
int is_adjacent(uintptr_t addr1, uintptr_t addr2, size_t size1, size_t size2);

#endif  // MEM_IMPL_H_
