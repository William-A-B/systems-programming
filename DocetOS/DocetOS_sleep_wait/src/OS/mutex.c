#include "OS/mutex.h"
#include "OS/os.h"

/* Acquire the mutex to prevent other tasks from running */
void OS_mutex_acquire(OS_mutex_t *mutex) {
	uint32_t failure = 0;
	OS_TCB_t *current_TCB = OS_currentTCB();
	uint32_t notification_counter;
	do {
		// Get latest notification count
		notification_counter = get_notification_counter();
		// Load task which is holding current mutex
		OS_TCB_t *task = (OS_TCB_t *) __LDREXW ((uint32_t *)&(mutex->TCB_task));
		// No task is holding the mutex
		if (task == 0) {
			// Acquire mutex for the current task and store current task in the mutex
			failure = __STREXW ((uint32_t) current_TCB, (uint32_t *)&(mutex->TCB_task));
		}
		else if (task != current_TCB) {
			// If mutex is held by another task, put current task on hold.
			// If notification counter held is different to latest notification counter, then notify has been called so don't hold task as mutex is free
			OS_wait(notification_counter);
			failure = 1;
		}
		else {
			// Same task is acquiring the mutex again
			failure = 0;
		}
		if (!failure) {
			// Increment counter as mutex has been acquired
			mutex->counter++;
		}
	} while (failure);
}

/* Release mutex and finish task */
void OS_mutex_release(OS_mutex_t *mutex) {
	// TODO WAB - do we need to check if the counter goes below zero?
	if (mutex->TCB_task == OS_currentTCB()) {
		// Decrement counter to release mutex
		mutex->counter--;
		// Mutex is no longer acquired by any task, so notify all tasks and yield to cause context switch
		if (!mutex->counter) {
			mutex->TCB_task = 0;
			OS_notifyAll();
			OS_yield();
		}
	}
}