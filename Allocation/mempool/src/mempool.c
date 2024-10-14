#include "mempool.h"

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
