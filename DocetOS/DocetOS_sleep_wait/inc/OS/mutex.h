#ifndef MUTEX_H
#define MUTEX_H

#include "scheduler.h"

typedef struct {
	OS_TCB_t *TCB_task;
	uint32_t counter;
} OS_mutex_t;

// Static initialiser for mutex
#define OS_MUTEX_STATIC_INITIALISER { .TCB_task = 0, .counter = 0 };

/* Acquire the mutex to prevent other tasks from running */
void OS_mutex_acquire(OS_mutex_t *mutex);

/* Release mutex and finish task */
void OS_mutex_release(OS_mutex_t *mutex);

#endif /* MUTEX_H */