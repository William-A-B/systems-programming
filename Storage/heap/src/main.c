#include "Utils/utils.h"
#include <stdio.h>
#include <inttypes.h>
#include "heap.h"

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	int32_t store[20] = {0};
	heap_t heap = HEAP_INITIALISER(store);
	printf("Testing\r\n");
	
	printf("Heap size = %" PRId32 "\r\n", get_heap_size(&heap));

	// Remember not to insert so many things that the heap overflows!
	heap_insert(&heap, 2);
	heap_insert(&heap, 4);
	heap_insert(&heap, 8);
	heap_insert(&heap, 9);
	heap_insert(&heap, 7);
	heap_insert(&heap, 1);
	heap_insert(&heap, 4);
	heap_insert(&heap, 1);
	heap_insert(&heap, 3);
	heap_insert(&heap, 2);
	heap_insert(&heap, 8);
	heap_insert(&heap, 3);
	heap_insert(&heap, 2);
	heap_insert(&heap, 1);
	heap_insert(&heap, 2);
	heap_insert(&heap, 3);
	heap_insert(&heap, 7);
	heap_insert(&heap, 6);
	heap_insert(&heap, 6);
	heap_insert(&heap, 1);
	
	printf("Heap size = %" PRId32 "\r\n", get_heap_size(&heap));
	
	for (uint32_t i = 0; i < get_heap_size(&heap); i++) {
		printf("Heap index %" PRId32 " = %" PRId32 "\r", i, store[i]);
	}
	printf("Extracted %" PRId32 "\r\n", heap_extract(&heap));
	for (uint32_t i = 0; i < get_heap_size(&heap); i++) {
		printf("Heap index %" PRId32 " = %" PRId32 "\r", i, store[i]);
	}
	printf("Extracted %" PRId32 "\r\n", heap_extract(&heap));
	for (uint32_t i = 0; i < get_heap_size(&heap); i++) {
		printf("Heap index %" PRId32 " = %" PRId32 "\r", i, store[i]);
	}
	while (!heap_isEmpty(&heap)) {
		printf("Extracted %" PRId32 "\r\n", heap_extract(&heap));
	}

	while(1);
}
