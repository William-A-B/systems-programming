#include "queue.h"

int32_t queue_put(queue_t * queue, int32_t data) {
	
	// Queue full so return false as cannot fit anymore data in queue
	if ((queue->insert + 1) % QUEUE_SIZE == queue->remove){
		return 0;
	}
	// Queue is not full so data can be added
	queue->data[queue->insert] = data;
	queue->insert = (queue->insert + 1) % QUEUE_SIZE;
	
	return 1;
}

int32_t queue_get(queue_t * queue, int32_t *data) {
	
	// Queue empty so return false as cannot access any data
	if (queue->remove == queue->insert) {
		return 0;
	}
	// Queue not empty so place data into pointer, increment remove position and return success
	*data = queue->data[queue->remove];
	queue->remove = (queue->remove + 1) % QUEUE_SIZE;
	
	return 1;
}
