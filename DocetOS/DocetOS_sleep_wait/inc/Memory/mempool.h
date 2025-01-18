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

/* Allocates a block from the pool */
void *pool_allocate(mempool_t *pool, OS_mutex_t *mutex);

/* Returns a block to the pool, deallocating the memory */
void pool_deallocate(mempool_t *pool, void *block, OS_mutex_t *mutex);

/* Initialises the memory pool to a given blocksize and number of blocks */
void pool_init(mempool_t *pool, size_t blocksize, size_t blocks);

#endif /* MEMPOOL_H */
