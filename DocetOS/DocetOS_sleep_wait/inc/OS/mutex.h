#ifndef MUTEX_H
#define MUTEX_H

#include "scheduler.h"

typedef struct {
	// Task that holds the mutex
	OS_TCB_t *TCB_task;
	// Number of times the mutex has been re-acquired
	uint32_t counter;
} OS_mutex_t;

// Static initialiser for mutex
#define OS_MUTEX_STATIC_INITIALISER { .TCB_task = 0, .counter = 0 };

/**
 * @brief Acquire the mutex to prevent other tasks from running
 * If the same task is trying to acquire the mutex again, 
 * it will not be blocked and will be allowed to acquire the mutex again
 * 
 * @param mutex - The mutex to attempt to acquire
 */
void OS_mutex_acquire(OS_mutex_t *mutex);

/**
 * @brief Release the mutex and notify all waiting tasks
 * Yield to cause context switch and switch tasks
 * 
 * @param mutex - The mutex to release
 */
void OS_mutex_release(OS_mutex_t *mutex);

#endif /* MUTEX_H */