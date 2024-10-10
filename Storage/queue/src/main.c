#include "Utils/utils.h"
#include "queue.h"
#include <inttypes.h>
#include <stdio.h>

queue_t queue = QUEUE_INITIALISER;

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	int32_t data = 0;
	
	for (int32_t i = 0; i < 9; i++) {
		if (queue_put(&queue, i)) {
			printf("Successfluly added %" PRId32 " to the queue\r\n", i);
		}
		else {
			printf("Cannot add data, queue is full!\r\n");
		}
	}
	
	
	for (int32_t i = 0; i < 12; i++) {
		if (queue_get(&queue, &data)) {
			printf("Removed %" PRId32 " from queue\r\n", data);
		}
		else {
			printf("Cannot remove data, queue is empty!\r\n");
		}
		
	}
}
