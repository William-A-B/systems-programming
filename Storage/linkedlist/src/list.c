#include "list.h"
#include <stdlib.h>

void list_insert(list_t *list, list_item_t *item) {
	// List is empty
	if (list->head == 0) {
		list->head = item;
		item->next = item;
		item->prev = item;
		return;
	}
	
	while (list->head->next != NULL) {
		
	}
}

void list_remove(list_t *list, list_item_t *item) {
	// To do...
}

list_item_t * list_next(list_t *list) {
	// To do...
}
