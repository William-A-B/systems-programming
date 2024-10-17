#include "mempool.h"
#include "static_alloc.h"
#include <inttypes.h>

void *pool_allocate(mempool_t *pool) {
	// Return the head of the list of blocks
	// Update the head pointer
	
	// Pool empty so return null
	if (pool->head == 0) {
		return 0;
	}
	
	mempool_item_t *block = pool->head;
	pool->head = pool->head->next;
	return block;
}

void pool_deallocate(mempool_t *pool, void *block) {
	// Add the new item to the head of the list
	mempool_item_t *mempool_block = block;
	mempool_block->next = pool->head;
	pool->head = mempool_block;
}

void pool_init(mempool_t *pool, size_t blocksize, size_t blocks) {
	
	blocksize = (~(STATIC_ALLOC_ALIGNMENT-1))&(blocksize+(STATIC_ALLOC_ALIGNMENT-1));
	
	uint32_t *static_pool_index = static_alloc(blocks * sizeof(blocksize));
	
	if (static_pool_index == 0) {
		pool->head = 0;
		return;
	}
	
	for (size_t i = 0; i < blocks; i++) {
		void *block = (void *)(static_pool_index + (i*blocksize));
		pool_add(pool, block);
	}
}