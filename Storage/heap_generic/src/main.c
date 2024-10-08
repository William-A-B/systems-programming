#include "Utils/utils.h"
#include <stdio.h>
#include <inttypes.h>
#include "heap.h"
#include "string.h"

typedef struct {
	char *name;
	uint32_t priority;
} example_item_t;

static int_fast8_t compare_heap(void *item_one, void *item_two) {
	example_item_t *ei_item_one = item_one;
	example_item_t *ei_item_two = item_two;
	if (ei_item_one->priority > ei_item_two->priority) {
		return 1;
	}	else if (ei_item_one->priority < ei_item_two->priority) {
		return -1;
	}
	return 0;
}

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	void *store[20];
	heap_t heap = HEAP_INITIALISER(store, compare_heap);

	// Remember not to insert so many things that the heap overflows!
	heap_insert(&heap, &(example_item_t){.name="first", .priority=2});
	heap_insert(&heap, &(example_item_t){.name="second", .priority=4});
	heap_insert(&heap, &(example_item_t){.name="third", .priority=3});
	heap_insert(&heap, &(example_item_t){.name="fourth", .priority=5});
	heap_insert(&heap, &(example_item_t){.name="fifth", .priority=6});
	heap_insert(&heap, &(example_item_t){.name="sixth", .priority=1});
	
	while (!heap_isEmpty(&heap)) {
		example_item_t *item = heap_extract(&heap);
		printf("Extracted %s (%" PRId32 ")\r\n", item->name, item->priority);
	}

	while(1);
}
