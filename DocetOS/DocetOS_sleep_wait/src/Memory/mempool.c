#include "Memory/mempool.h"
#include "Memory/static_alloc.h"
#include <inttypes.h>


/**
 * @brief Allocates a block of memory from the memory pool
 * 
 * @param pool - Memory pool to allocate from
 * @param mutex - Mutex to protect the pool from concurrent modification
 * @return void* - Pointer to the allocated block of memory
 */
void *pool_allocate(mempool_t *pool, OS_mutex_t *mutex) {
	OS_mutex_acquire(mutex);
	// Pool empty so return null
	if (pool->head == 0) {
		return 0;
	}
	// Allocates a block by returning the current head
	// Get current head block of pool
	mempool_item_t *block = pool->head;
	// Update the head pointer to next block
	pool->head = pool->head->next;
	OS_mutex_release(mutex);
	return block;
}

/**
 * @brief Returns a block to the pool, deallocating the memory
 * 
 * @param pool - Memory pool to deallocate from
 * @param block - Block of memory to deallocate
 * @param mutex - Mutex to protect the pool from concurrent modification
 */
void pool_deallocate(mempool_t *pool, void *block, OS_mutex_t *mutex) {
	OS_mutex_acquire(mutex);
	// Point next block to head
	mempool_item_t *mempool_block = block;
	mempool_block->next = pool->head;
	// Update head to point to new free block
	pool->head = mempool_block;
	OS_mutex_release(mutex);
}

/**
 * @brief Inline function to add blocks of memory to the pool to be used 
 * Equivalent to pool_deallocate, however only called in the initialisation
 * so does not require a mutex.
 */
static inline void pool_add(mempool_t *pool, void *block) {
	// Point next block to head
	mempool_item_t *mempool_block = block;
	mempool_block->next = pool->head;
	// Update head to point to new free block
	pool->head = mempool_block;
}

/**
 * @brief Initialises the memory pool to a given blocksize and number of blocks
 * 
 * @param pool - A pointer to the memory pool to initialise
 * @param blocksize - Size of each block in the pool (bytes)
 * @param blocks - Number of blocks to allocate in the pool
 */
void pool_init(mempool_t *pool, size_t blocksize, size_t blocks) {
	// Align the blocksize to 8 bytes
	blocksize = (~(STATIC_ALLOC_ALIGNMENT-1))&(blocksize+(STATIC_ALLOC_ALIGNMENT-1));
	
	// Allocate a contiguous block of memory 
	uint32_t *static_pool_index = static_alloc(blocks * sizeof(blocksize));
	
	// Allocation failed, number of bytes too large for poolsize
	if (static_pool_index == 0) {
		pool->head = 0;
		return;
	}
	// Allocation successful, divide memory into blocks and adds these blocks into the pool
	for (size_t i = 0; i < blocks; i++) {
		// Calculate address of current block
		void *block = (void *)(static_pool_index + (i*blocksize));
		pool_add(pool, block);
	}
}