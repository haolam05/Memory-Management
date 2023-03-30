// File Purpose	: implementation of function check_heap and other functions that used internally

// INCLUDES LIBRARIES
#include <assert.h>

// INCLUDES FILES
#include "mem_impl.h"

// DO:
//  1) Check for possible problems with the free list data structure
//     by verifying that the free list has the following properties:
//     + Blocks are ordered with strictly increasing memory addresses
//     + Block sizes > 0 and no smaller than specified minimum block size
//     + Blocks do not overlap
//     + Blocks are not touching
//  => returns silently if no errors are found, otherwise terminates program
// Parameters:  NONE
void check_heap() {
    struct free_node *curr = freelist;

    while (curr && curr->next) {
        // Blocks are ordered with strictly increasing memory addresses
        assert(curr < (curr->next));

        // Block sizes > 0 and no smaller than specified minimum block size
        assert(curr->size > 0);

        // Blocks do not overlap & do not touch
        assert((uintptr_t)curr + HEADER_SIZE + curr->size
                < (uintptr_t)(curr->next));

        curr = curr->next;
    }
}

// DO:
//  1) ask underlying operating system for a giant block of memory(malloc)
// Parameters:
//  uintptr_t size: the size of the memory block to be allocated
struct free_node * newMemBlock(uintptr_t size) {
    total_bytes_allocated += size + HEADER_SIZE;
    struct free_node *newBlock = (struct free_node *)malloc(size + HEADER_SIZE);
    newBlock->size = size;
    newBlock->next = NULL;
    return newBlock;
}

// DO:
//  1) iterate through freelist and combine adjacent blocks if found any
// Parameters: NONE
void combine_if_found_adjacents() {
    struct free_node *curr = freelist;

    while (curr->next) {
        if (is_adjacent((uintptr_t)curr, (uintptr_t)curr->next,
                         curr->size, curr->next->size)) {
            curr->size += curr->next->size + HEADER_SIZE;
            curr->next = curr->next->next;
            free(curr->next);
        } else {
            curr = curr->next;
        }
    }
}

// DO:
//  1) check if two blocks are adjacent
// Parameters:
//  uintptr_t addr1: the address of the first   block
//  uintptr_t addr2: the address of the second  block
//  uintptr_t size1: the size    of the first   block
//  uintptr_t size2: the size    of the second  block
int is_adjacent(uintptr_t addr1, uintptr_t addr2, size_t size1, size_t size2) {
    return (addr1 > addr2) ? (addr1 == addr2 + size2 + HEADER_SIZE) :
                             (addr2 == addr1 + size1 + HEADER_SIZE);
}

// DO:
//  1) put the new block on the freelist in the correct order (ASC)
// Parameters: NONE
void putNewBlockOnFreelist(struct free_node *newBlock) {
    struct free_node *curr = freelist;

    if (curr == NULL) {             // CASE1: freelist is empty
        freelist = newBlock;
    } else if (newBlock < curr) {   // CASE2: prepend
        freelist = newBlock;
        freelist->next = curr;
    } else {                        // CASE3: append
        while (curr->next && newBlock > curr->next) {
            curr = curr->next;
        }

        newBlock->next = curr->next;
        curr->next = newBlock;
    }
}

// DO:
//  1) returns a pointer to a block with a given size on freelist
// Parameters:
//  uintptr_t size          : the size of the memory block to be allocated
//  struct free_node *curr  : pointer to current free block
//  struct free_node *prev  : pointer to previous     block
void * findBlock(uintptr_t size, struct free_node *curr,
                                 struct free_node *prev) {
    struct free_node *new_ptr;

    // split block if > 20% of requested size
    if ((int)(size * MAX_LARGER_PCT) + HEADER_SIZE < curr->size) {
        curr->size = curr->size - size;
        new_ptr = (struct free_node *)((uintptr_t)curr +
                                        (2 * HEADER_SIZE) + curr->size);
    } else {                                // no split
        new_ptr = (struct free_node *)((uintptr_t)curr + HEADER_SIZE);

        if (curr == freelist) {             // first block
            freelist->next = curr->next;
        } else {                            // not first block
            prev->next = curr->next;
        }
    }

    new_ptr->size = size;
    new_ptr->next = NULL;
    return (void *) new_ptr;
}
