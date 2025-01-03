#ifndef MUTEX_H
#define MUTEX_H

#include "scheduler.h"

typedef struct {
	OS_TCB_t *TCB_task;
	uint32_t counter;
} OS_mutex_t;

#define OS_MUTEX_STATIC_INITIALISER { .TCB_task = 0, .counter = 0 };

void OS_mutex_acquire(OS_mutex_t *mutex);

void OS_mutex_release(OS_mutex_t *mutex);

#endif /* MUTEX_H */