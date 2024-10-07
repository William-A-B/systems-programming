#include "heap.h"

static void heap_up(heap_t *heap) {
	// Start with the last element in the heap
	// If it's the root element, stop
	// Compare it with its parent
	// If the parent is smaller or equal, stop
	// Swap the element with its parent
	// With the element in its new locaiton, go back to step 2
	
	// 0 = even
	// 1 = odd
	int32_t n = -1;
	int32_t element = 0;
	int32_t parent_element = 0;
	
	int32_t * const offsetStore = heap->store - 1;
	
	// Use variables to keep track of current and next ones and check if root element each time
	while (heap->store[0] != heap->store[heap->size]) {
		n = heap->size % 2;
		
		// Element is the root element
		if (heap->store[0] == heap->store[heap->size]) {
			return;
		}
		
		if (n == 0) {
			element = heap->store[heap->size];
			parent_element = heap->store[(heap->size/2)];
			if (heap->store[heap->size] > heap->store[(heap->size/2)]) {
				return;
			}
			else if (heap->store[heap->size] < heap->store[(heap->size/2)]) {
				heap->store[(heap->size/2)] = element;
				heap->store[heap->size] = parent_element;
			}
		}
		else if (n == 1) {
			element = heap->store[heap->size];
			parent_element = heap->store[((heap->size-1)/2)];
			if (heap->store[heap->size] > heap->store[((heap->size-1)/2)]) {
				return;
			}
			else if (heap->store[heap->size] < heap->store[((heap->size-1)/2)]) {
				heap->store[((heap->size-1)/2)] = element;
				heap->store[heap->size] = parent_element;
			}
		}
	}
}

static void heap_down(heap_t *heap) {
	// Write me!
}

void heap_insert(heap_t *heap, int32_t value) {
	// The new element is always added to the end of a heap
	heap->store[(heap->size)++] = value;
	heap_up(heap);
}

int32_t heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	int value = heap->store[0];
	heap->store[0] = heap->store[--(heap->size)];
	heap_down(heap);
	return value;
}

uint_fast8_t heap_isEmpty(heap_t *heap) {
	return !(heap->size);
}
