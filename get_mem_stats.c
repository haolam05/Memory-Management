// File Purpose	: implementation of function get_mem_stats

// INCLUDES FILES
#include "mem_impl.h"

// DO:
//  1) stores statistics about the current state of the memory manager
// Parameters:
//      total_size      : the total amount of storage in bytes requested
//                          from the underlying system
//      total_free      : the total amount of storage in bytes that is
//                          currently stored on the free list
//      n_free_blocks   : the total number of individual blocks currently
//                          stored on the free list.
void get_mem_stats(uintptr_t *total_size, uintptr_t *total_free,
                   uintptr_t *n_free_blocks) {
    struct free_node *curr    = freelist;
    uintptr_t num_free_blocks = 0;
    uintptr_t num_free_bytes  = 0;

    while (curr) {
        num_free_bytes += curr->size;
        num_free_blocks++;
        curr = curr->next;
    }

    *total_size     = total_bytes_allocated;
    *total_free     = num_free_bytes;
    *n_free_blocks  = num_free_blocks;
}
