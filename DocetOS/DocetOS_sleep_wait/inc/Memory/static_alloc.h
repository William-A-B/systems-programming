#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H
#include <stddef.h>

// Align all allocations to 8 bytes
#define STATIC_ALLOC_ALIGNMENT ((size_t) 8)

/* Allocates a set number of byes from the memory pool */
void * static_alloc(size_t bytes);

#endif /* STATIC_ALLOC_H */
