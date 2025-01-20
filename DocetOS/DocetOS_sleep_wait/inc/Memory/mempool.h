#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stddef.h>
#include "OS/mutex.h"

// A block of memory in the pool
struct mempool_item_s {
	struct mempool_item_s *next;
};
typedef struct mempool_item_s mempool_item_t;

// Head pointer to the main pool object
typedef struct {
	mempool_item_t *head;
} mempool_t;

// Static initialiser for the memory pool
#define MEMPOOL_INITIALISER { .head = 0 }

/**
 * @brief Allocates a block of memory from the memory pool
 * 
 * @param pool - Memory pool to allocate from
 * @param mutex - Mutex to protect the pool from concurrent modification
 * @return void* - Pointer to the allocated block of memory
 */
void *pool_allocate(mempool_t *pool, OS_mutex_t *mutex);

/**
 * @brief Returns a block to the pool, deallocating the memory
 * 
 * @param pool - Memory pool to deallocate from
 * @param block - Block of memory to deallocate
 * @param mutex - Mutex to protect the pool from concurrent modification
 */
void pool_deallocate(mempool_t *pool, void *block, OS_mutex_t *mutex);

/**
 * @brief Initialises the memory pool to a given blocksize and number of blocks
 * 
 * @param pool - A pointer to the memory pool to initialise
 * @param blocksize - Size of each block in the pool (bytes)
 * @param blocks - Number of blocks to allocate in the pool
 */
void pool_init(mempool_t *pool, size_t blocksize, size_t blocks);

#endif /* MEMPOOL_H */
