#include "static_alloc.h"
#include <stdint.h>

// 16KB pool size
#define STATIC_ALLOC_POOLSIZE 16384UL
#define STATIC_ALLOC_ALIGNMENT ((size_t) 8)

static uint8_t static_pool[STATIC_ALLOC_POOLSIZE] __attribute__ (( aligned(STATIC_ALLOC_ALIGNMENT) ));
static size_t pool_index = STATIC_ALLOC_POOLSIZE;

void * static_alloc(size_t bytes) {
	
	
	if (bytes < pool_index) {
		pool_index = (~(STATIC_ALLOC_ALIGNMENT - 1))&(pool_index - bytes);
		return &pool_index;
	}
	return 0;
}
