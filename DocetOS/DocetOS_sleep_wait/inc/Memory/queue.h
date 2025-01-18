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

/* Add an element to the queue */
void queue_put(queue_t * queue, uint32_t *data, OS_mutex_t *mutex);

/* Retrieve an element from the queue */
void queue_get(queue_t * queue, uint32_t **data, OS_mutex_t *mutex);

#endif /* QUEUE_H */
