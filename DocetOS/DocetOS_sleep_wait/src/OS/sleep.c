#define OS_INTERNAL
#include "OS/sleep.h"
#include "OS/os.h"


void OS_sleep(uint32_t sleep_time) {
	
	// Get the current running task's TCB
	OS_TCB_t *current_TCB = OS_currentTCB();
	// Set the wakeup time of the task to be the total number of pre-emption ticks since last reset plus the sleep time
	current_TCB->wakeup_time = OS_elapsedTicks() + sleep_time;
	// Set bit one of the state variable to indicate the task is asleep
	current_TCB->state = current_TCB->state | TASK_STATE_SLEEP; 
	// Initiate task switch
	OS_yield();
}
	