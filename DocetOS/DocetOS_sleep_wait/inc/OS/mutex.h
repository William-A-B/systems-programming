#ifndef MUTEX_H
#define MUTEX_H

#include "scheduler.h"

uint32_t __LDREXW (uint32_t *addr);
uint32_t __STREXW (uint32_t value, uint32_t *addr);

typedef struct {
	OS_TCB_t *TCB_task;
	uint32_t counter;
} OS_mutex_t;

#define OS_MUTEX_STATIC_INITIALISER() { .TCB_task = 0, .counter = 0 }

void OS_mutex_acquire(void);

void OS_mutex_release(void);

#endif /* MUTEX_H */