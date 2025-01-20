#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H
#include <stddef.h>

// Align all allocations to 8 bytes
#define STATIC_ALLOC_ALIGNMENT ((size_t) 8)

/**
 * @brief Allocates a set number of byes from the memory pool
 * 
 * @param bytes - Number of bytes to allocate
 * @return void* - Pointer to the index of the allocated pool of memory
 */
void * static_alloc(size_t bytes);

#endif /* STATIC_ALLOC_H */
