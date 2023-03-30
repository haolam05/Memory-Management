// File Purpose	: implementation of function getmem

// INCLUDES FILES
#include "mem_impl.h"
#include "mem.h"

// DO:
//  1) returns a pointer to a block of memory of at least size bytes of memory
//  2) the pointer to the returned block should be aligned
//      on an 16-byte boundary
// Parameters:
//  uintptr_t size: the size of the memory block to be allocated
//                  (must be positive)
void* getmem(uintptr_t size) {
    check_heap();

    if (size <= 0) { return NULL; }

    struct free_node *curr = freelist;
    struct free_node *prev = freelist;

    // search for a block on freelist that is big enough
    while (curr && curr->size < size) {
        prev = curr;
        curr = curr->next;
    }

    // found a block on freelist that is big enough
    if (curr) { return findBlock(size, curr, prev); }

    // if not found, ask underlying system for more memory
    uintptr_t new_size = DEFAULT_MALLOC_SIZE + size +
                        (MIN_BLOCK_SIZE - (size % MIN_BLOCK_SIZE));
    struct free_node *newBlock = newMemBlock(new_size);
    putNewBlockOnFreelist(newBlock);
    combine_if_found_adjacents();
    return getmem(size);  // search again

    check_heap();
}
