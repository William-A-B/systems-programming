#include "Memory/queue.h"
#include "OS/semaphore.h"


static OS_semaphore_t queue_not_full_semaphore = OS_SEMAPHORE_STATIC_INITIALISER(QUEUE_SIZE);
static OS_semaphore_t queue_not_empty_semaphore = OS_SEMAPHORE_STATIC_INITIALISER(QUEUE_SIZE);

/* Add an element to the queue */
uint32_t queue_put(queue_t * queue, uint32_t *data, OS_mutex_t *mutex) {
	OS_semaphore_obtain(&queue_not_full_semaphore);
	OS_semaphore_release(&queue_not_empty_semaphore);
	OS_mutex_acquire(mutex);
	
//	// Queue full so return false as cannot fit anymore data in queue
//	if ((queue->insert + 1) % QUEUE_SIZE == queue->remove){
//		OS_mutex_release(mutex);
//		return 0;
//	}
	// Queue is not full so data can be added
	queue->data[queue->insert] = data;
	queue->insert = (queue->insert + 1) % QUEUE_SIZE;
	OS_mutex_release(mutex);
	return 1;
}

/* Retrieve an element from the queue */
uint32_t queue_get(queue_t * queue, uint32_t **data, OS_mutex_t *mutex) {
	OS_semaphore_obtain(&queue_not_empty_semaphore);
	OS_semaphore_release(&queue_not_full_semaphore);
	OS_mutex_acquire(mutex);
//	// Queue empty so return false as cannot access any data
//	if (queue->remove == queue->insert) {
//		OS_mutex_release(mutex);
//		return 0;
//	}
	// Queue not empty so place data into pointer, increment remove position and return success
	*data = queue->data[queue->remove];
	queue->remove = (queue->remove + 1) % QUEUE_SIZE;
	OS_mutex_release(mutex);
	return 1;
}
