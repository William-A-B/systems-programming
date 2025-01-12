#include "Memory/static_alloc.h"
#include <stdint.h>

// 16KB pool size
#define STATIC_ALLOC_POOLSIZE 16384UL

static uint8_t static_pool[STATIC_ALLOC_POOLSIZE] __attribute__ (( aligned(STATIC_ALLOC_ALIGNMENT) ));
static size_t pool_index = STATIC_ALLOC_POOLSIZE;

void * static_alloc(size_t bytes) {
	
	// Requested num bytes fits within remaining pool
	if (bytes < pool_index) {
		// Adjust pool index to align by 8 bytes and reduce it by the number of requested bytes
		pool_index = (~(STATIC_ALLOC_ALIGNMENT - 1))&(pool_index - bytes);
		return &static_pool[pool_index];
	}
	return 0;
}
