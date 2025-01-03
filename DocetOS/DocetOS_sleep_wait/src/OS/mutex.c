#include "OS/mutex.h"
#include "OS/os.h"

/* Acquire the mutex to prevent other tasks from running */
void OS_mutex_acquire(OS_mutex_t *mutex) {
	uint32_t failure = 0;
	OS_TCB_t *current_TCB = OS_currentTCB();
	uint32_t notification_counter;
	do {
		notification_counter = get_notification_counter();
		OS_TCB_t *task = (OS_TCB_t *) __LDREXW ((uint32_t *)&(mutex->TCB_task));
		if (task == 0) {
			failure = __STREXW ((uint32_t) current_TCB, (uint32_t *)&(mutex->TCB_task));
		}
		else if (task != current_TCB) {
			OS_wait(notification_counter);
			failure = 1;
		}
		else {
			failure = 0;
		}
		if (!failure) {
			mutex->counter++;
		}
	} while (failure);
}

/* Release mutex and finish task */
void OS_mutex_release(OS_mutex_t *mutex) {
	// TODO WAB - do we need to check if the counter goes below zero?
	if (mutex->TCB_task == OS_currentTCB()) {
		mutex->counter--;
		if (mutex->counter == 0) {
			mutex->TCB_task = 0;
			OS_notifyAll();
			OS_yield();
		}
	}
}