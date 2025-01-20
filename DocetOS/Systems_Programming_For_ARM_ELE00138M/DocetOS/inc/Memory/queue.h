#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "OS/mutex.h"

#define QUEUE_SIZE 10

typedef struct {
	uint32_t *data[QUEUE_SIZE];
	uint_fast16_t insert;
	uint_fast16_t remove;
} queue_t;

#define QUEUE_INITIALISER { .data = {0}, .insert = 0, .remove = 0 }

/**
 * @brief Add an element to the queue
 * If the queue is full, the calling task will wait until there is space
 * 
 * @param queue - Queue to add data to
 * @param data - Data to add to the queue
 * @param mutex - Mutex to protect the queue from concurrent modification
 */
void queue_put(queue_t * queue, uint32_t *data, OS_mutex_t *mutex);

/**
 * @brief Retrieve an element from the queue
 * If the queue is empty, the calling task will wait until there is data available
 * 
 * @param queue - Queue to retrieve data from
 * @param data - Pointer to store the data retrieved from the queue
 * @param mutex - Mutex to protect the queue from concurrent modification
 */
void queue_get(queue_t * queue, uint32_t **data, OS_mutex_t *mutex);

#endif /* QUEUE_H */
