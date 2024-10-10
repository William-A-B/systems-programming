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
	
	// List not empty, insert at previous address
	list_item_t *first_item = list->head;
	list_item_t *last_item = list->head->prev;
	
	item->next = first_item;
	item->prev = last_item;
	first_item->prev = item;
	last_item->next = item;
}

void list_remove(list_t *list, list_item_t *item) {
	// Item at head of list
	if (list->head == item) {
		// Item is the only item in the list
		if (item->next == item) {
			list->head = 0;
			return;
		}
		// Remove item and update pointers
		// Point head to next item
		item->next->prev = item->prev;
		item->prev->next = item->next;
		list->head = item->next;
		return;
	}
	
	list_item_t *prev_item = list->head;
	// Iterate until previous item is equal to the previous of the desired item
	while (prev_item != item->prev) {
		prev_item = prev_item->next;
	}
	// Remove item and update pointers
	prev_item->next = item->next;
	item->next->prev = prev_item;
}

list_item_t * list_next(list_t *list) {
	// Get current head item
	list_item_t *item = list->head;
	// Point head to next item
	list->head = item->next;
	// Return item
	return item;
}
