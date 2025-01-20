#include "Memory/queue.h"
#include "OS/semaphore.h"

// Counting Semaphores to allow synchronisation when inserting and removing from the queue
// Semaphores are initialised to the maximum queue size and minimum queue size
static OS_semaphore_t queue_not_full_semaphore = OS_SEMAPHORE_STATIC_INITIALISER(QUEUE_SIZE, QUEUE_SIZE);
static OS_semaphore_t queue_not_empty_semaphore = OS_SEMAPHORE_STATIC_INITIALISER(QUEUE_SIZE, 0);

/**
 * @brief Add an element to the queue
 * If the queue is full, the calling task will wait until there is space
 * 
 * @param queue - Queue to add data to
 * @param data - Data to add to the queue
 * @param mutex - Mutex to protect the queue from concurrent modification
 */
void queue_put(queue_t * queue, uint32_t *data, OS_mutex_t *mutex) {
	// Obtain token to verify there is space in the queue
	// If queue is full, calling task will wait
	OS_semaphore_obtain(&queue_not_full_semaphore);
	OS_mutex_acquire(mutex);
	// Queue is not full so data can be added
	queue->data[queue->insert] = data;
	queue->insert = (queue->insert + 1) % QUEUE_SIZE;
	OS_mutex_release(mutex);
	// Add token into not_empty semaphore to indicate one queue space has been filled
	OS_semaphore_release(&queue_not_empty_semaphore);
}

/**
 * @brief Retrieve an element from the queue
 * If the queue is empty, the calling task will wait until there is data available
 * 
 * @param queue - Queue to retrieve data from
 * @param data - Pointer to store the data retrieved from the queue
 * @param mutex - Mutex to protect the queue from concurrent modification
 */
void queue_get(queue_t * queue, uint32_t **data, OS_mutex_t *mutex) {
	// Obtain token to verify that the queue isn't empty
	// If empty calling task will wait
	OS_semaphore_obtain(&queue_not_empty_semaphore);
	OS_mutex_acquire(mutex);
	// Queue not empty so place data into pointer, increment remove position and return success
	*data = queue->data[queue->remove];
	queue->remove = (queue->remove + 1) % QUEUE_SIZE;
	OS_mutex_release(mutex);
	// Add token into not_full semaphore to indicate one queue space has been emptied
	OS_semaphore_release(&queue_not_full_semaphore);
}
