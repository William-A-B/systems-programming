#include "heap.h"

static int32_t get_children_count(uint32_t heap_size, uint32_t element_index) {
	if ((element_index*2) > heap_size) {
		return 0;
	}
	else if (((element_index*2)+1) <= heap_size) {
		return 2;
	}
	else if ((element_index*2) <= heap_size) {
		return 1;
	}
	return 0;
}

static void swap_elements(int32_t *store, uint32_t index_one, uint32_t index_two) {
	int32_t element_one = store[index_one];
	int32_t element_two = store[index_two];
	store[index_two] = element_one;
	store[index_one] = element_two;
}


static void heap_up(heap_t *heap) {
	// Start with the last element in the heap
	// If it's the root element, stop
	// Compare it with its parent
	// If the parent is smaller or equal, stop
	// Swap the element with its parent
	// With the element in its new locaiton, go back to step 2
	
	int32_t element = 0;
	int32_t parent_element = 0;
	uint32_t element_index = 0;
	uint32_t parent_index = 0;
	
	// Allows arrays to be 1 indexed
	// Accessing offsetStore[0] will provide some random memory
	int32_t * const offsetStore = heap->store - 1;
	
	element_index = heap->size;
	parent_index = (element_index/2);
	
	
	// Use variables to keep track of current and next ones and check if root element each time
	while (element_index > 1) {
		
		parent_index = (element_index/2);
		element = offsetStore[element_index];
		parent_element = offsetStore[parent_index];
		
		if (element >= parent_element) {
			return;
		}
		swap_elements(offsetStore, element_index, parent_index);
		element_index = parent_index;
	}
}

static void heap_down(heap_t *heap) {
	// Start with the root element
	// If it has no children, stop
	// Compare it with its children
	// If the element is smaller than or equal to both children, stop
	// Swap the element with the smaller of the children
	// With the element in its new location, go back to step 2
	
	
	// 0 = even
	// 1 = odd
	int32_t element = 0;
	uint32_t element_index = 0;
	int32_t child_a = 0;
	int32_t child_b = 0;
	uint32_t child_a_index = 0;
	uint32_t child_b_index = 0;
	
	// Allows arrays to be 1 indexed
	// Accessing offsetStore[0] will provide some random memory
	int32_t * const offsetStore = heap->store - 1;
	
	element_index = 1;
	int32_t child_count = get_children_count(heap->size, element_index);
	
	while (child_count) {
		
		// Element is now the last element
		if (element_index >= heap->size) {
			return;
		}
		
		element = offsetStore[element_index];
		
		child_a_index = (2*element_index);
		child_a = offsetStore[child_a_index];
		
		if (child_count == 2) {
			child_b_index = ((2*element_index)+1);
			child_b = offsetStore[child_b_index];
			
			if (child_b < child_a) {
				if (element <= child_b) {
					return;
				}
				swap_elements(offsetStore, element_index, child_b_index);
				element_index = child_b_index;
			}
			else {	
				if (element <= child_a) {
					return;
				}
				swap_elements(offsetStore, element_index, child_a_index);
				element_index = child_a_index;
			}
		}
		else {
			if (element <= child_a) {
					return;
			}
			swap_elements(offsetStore, element_index, child_a_index);
			element_index = child_a_index;
		}
		
		child_count = get_children_count(heap->size, element_index);
	}
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

uint32_t get_heap_size(heap_t *heap) {
	return heap->size;
}
