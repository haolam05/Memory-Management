// File Purpose	: implementation of function print_heap

// INCLUDES LIBRARIES
#include <stdio.h>

// INCLUDES FILES
#include "mem_impl.h"

// DO:
//  1) prints a formatted listing on file f showing the blocks on the free list
//     (each line of the output describes one free block)
// Parameters:
//      File *f: the file to which the output should be printed
void print_heap(FILE * f) {
    struct free_node *curr = freelist;
    int block_num = 1;
    while (curr) {
        fprintf(f, "Block #%d\tSize: %lu bytes     \tLocation:\t%p\n",
                block_num, curr->size, (void *)curr);
        curr = curr->next;
        block_num++;
    }

    fprintf(f, "==================================\n");
    fprintf(f, "Total bytes allocated: %lu\n", total_bytes_allocated);
}
