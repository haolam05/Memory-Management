// File Purpose	: implementation of function freemem

// INCLUDES FILES
#include "mem_impl.h"

// DECLARATIONS
void combine_if_found_adjacents();
int is_adjacent(uintptr_t addr1, uintptr_t addr2, size_t size1, size_t size2);

// DO:
//  1) returns the block of storage at location p to the
//      pool of available free storage
//     (p must be a pointer returned by a previous call to getmem)
//     => error message will be printed to stdout and program will
//         crash if p is not a valid pointer
// Parameters:
//  void *p: the pointer to the block of memory to be freed
void freemem(void *p) {
    check_heap();

    if (p == NULL) { return; }
    uintptr_t pSize = ((struct free_node *)p)->size;
    if (pSize <= 0) { return; }
    struct free_node *newBlock = newMemBlock(pSize);
    putNewBlockOnFreelist(newBlock);
    combine_if_found_adjacents();

    check_heap();
}
