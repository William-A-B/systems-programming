#define OS_INTERNAL

#include "OS/scheduler.h"
#include "OS/os.h"

#include "stm32f4xx.h"
#include <stdio.h>

#include <string.h>

// SVC delegate function declarations
void _OS_wait_delegate(_OS_SVC_StackFrame_t const * const);
void _OS_sleep_delegate(_OS_SVC_StackFrame_t const * const);

static uint32_t notification_counter = 0;

/* This is an implementation of an extremely simple round-robin scheduler.

   A task list structure is declared.  Tasks are added to the list to create a circular buffer.
	 When tasks finish, they are removed from the list.  When the scheduler is invoked, it simply
	 advances the head pointer, and returns it.  If the head pointer is null, a pointer to the
	 idle task is returned instead.
	 
	 The scheduler is reasonably efficient but not very flexible.  The "yield" flag is not
	 checked, but merely cleared before a task is returned, so OS_yield() is equivalent to
	 OS_schedule() in this implementation.
*/

// Task lists for different states
static _OS_tasklist_t task_list = {.head = 0};
static _OS_tasklist_t wait_list = {.head = 0};
static _OS_tasklist_t pending_list = {.head = 0};
static _OS_tasklist_t sleep_list = {.head = 0};

static void print_task_list(_OS_tasklist_t *task_list) {
	if (!task_list->head) {
		printf("The list is empty.\n");
		return;
	}
	OS_TCB_t *current_task = task_list->head;
	do {
			printf("Task Priority: %d\n", current_task->priority);
			current_task = current_task->next;
	} while (current_task != task_list->head);
}

/* Add a task into the tasklist, sorted by priority */
static void list_add(_OS_tasklist_t *list, OS_TCB_t *task) {
	// Insert at head, nothing in list
	if (!(list->head)) {
		task->next = task;
		task->prev = task;
		list->head = task;
	} else {
		OS_TCB_t *current_task = list->head;
		
		do {
			// Check if task should be inserted before current task
			if (task->priority < current_task->priority) {
				task->next = current_task;
				task->prev = current_task->prev;
				current_task->prev->next = task;
				current_task->prev = task;
				
				// Update list head if new task has the highest priority
				if ((current_task == list->head) && (task->priority < list->head->priority)) {
					list->head = task;
				}
				return;
			}
			// Get the next task in the list
			current_task = current_task->next;
		} while (current_task != list->head);
		
		// Task priority is not less than any existing priority in the list
		// Insert at end of list
		task->next = list->head;
		task->prev = list->head->prev;
		list->head->prev->next = task;
		list->head->prev = task;
	}
}
/* Remove a task from a list */
static void list_remove(_OS_tasklist_t *list, OS_TCB_t *task) {
	// Item at head of list
	if (list->head == task) {
		// Item is the only item in the list
		if (task->next == task) {
			list->head = 0;
			return;
		}
		// Remove item and update pointers
		// Point head to next item
		task->next->prev = task->prev;
		task->prev->next = task->next;
		list->head = task->next;
		return;
	}
	
	OS_TCB_t *prev_task = list->head;
	// Iterate until previous item is equal to the previous of the desired item
	while (prev_task != task->prev) {
		prev_task = prev_task->next;
	}
	// Remove item and update pointers
	prev_task->next = task->next;
	task->next->prev = prev_task;
}

/* Add (push) given task onto the list provided */
static void list_push_sl(_OS_tasklist_t *list, OS_TCB_t *task) {
	do {
		OS_TCB_t *head = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
		task->next = head;
	} while (__STREXW ((uint32_t) task, (uint32_t *)&(list->head)));
}

/* Add (push) the given task onto the list provided, sorting it by sleep time */
static void list_sort_push_sl(_OS_tasklist_t *list, OS_TCB_t *task) {
	uint32_t failure = 1;
	do {
		OS_TCB_t *head = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
		// Task has shortest sleep time, so insert at top of list
		if (head == NULL || task->wakeup_time <= head->wakeup_time) {
			task->next = head;
			failure = __STREXW ((uint32_t) task, (uint32_t *)&(list->head));
		}
		else {
			OS_TCB_t *current_task = head;
			OS_TCB_t *next_task = head->next;
			do {
				// Check if task should be inserted before next task
				if (task->wakeup_time <= next_task->wakeup_time) {
					task->next = next_task;
					failure = __STREXW ((uint32_t) task, (uint32_t *)&(current_task->next));
					break;
				}
				else if (!next_task) {
					// No next task, insert at end of list
					task->next = 0;
					failure = __STREXW ((uint32_t) task, (uint32_t *)&(current_task->next));
					break;
				}
				// Get the next task in the list
				current_task = next_task;
				next_task = next_task->next;
			} while (1);
		}
	} while (failure);
}

/* Pop top task off the list and return it */
static OS_TCB_t *list_pop_sl(_OS_tasklist_t *list) {
	OS_TCB_t *head_task;
	do {
		head_task = (OS_TCB_t *) __LDREXW ((uint32_t *)&(list->head));
		if (!head_task) {
			__CLREX();
			return 0;
		}
	} while (__STREXW ((uint32_t) head_task->next, (uint32_t *)&(list->head)));
	return head_task;
}

void _OS_sleep_delegate(_OS_SVC_StackFrame_t const * const stack) {
	// Get the current running task's TCB
	OS_TCB_t * const current_TCB = OS_currentTCB();
	// Set the wakeup time of the task to be the total number of pre-emption ticks since last reset plus the sleep time
	current_TCB->wakeup_time = OS_elapsedTicks() + stack->r0;
	// Set bit one of the state variable to indicate the task is asleep
	current_TCB->state = current_TCB->state | TASK_STATE_SLEEP; 
	
	// Removes sleeping tasks from task list and adds into sleeping list
	list_remove(&task_list, current_TCB);
	list_sort_push_sl(&sleep_list, current_TCB);
	
	// Initiate task switch
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* Remove current task from task list and place it in waiting list */
void _OS_wait_delegate(_OS_SVC_StackFrame_t const * const stack) {
	// If current notification counter is different to provided notification counter, then task shouldn't be held
	if (stack->r0 != notification_counter) {
		return;
	}
	// Get current task and remove it from task list and place it in waiting list
	OS_TCB_t *current_task = OS_currentTCB();
	list_remove(&task_list, current_task);
	// Set the yield bit
	current_task->state |= TASK_STATE_YIELD;
	list_push_sl(&wait_list, current_task);
	// Set PendSV bit to trigger context switch
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* Notify all tasks in wait list and move them to pending list */
void OS_notifyAll(void) {
	// While wait list still has tasks in it
	// Remove all tasks and add them to the pending list
	OS_TCB_t *task;
	uint32_t notification_count_temp;
	do {
		notification_count_temp = (uint32_t) __LDREXW ((uint32_t *)&(notification_counter));
		notification_count_temp++;
	} while (__STREXW ((uint32_t) notification_count_temp, (uint32_t *)&(notification_counter)));
	while ((task = list_pop_sl(&wait_list))) {
		list_push_sl(&pending_list, task);
	}
}

/* Fixed Priority Round-robin scheduler */
OS_TCB_t const * _OS_schedule(void) {

	// While pending list is not empty, remove them and add them into the round robin
	while (pending_list.head != NULL) {
		// Retrieve top task from pending list
		OS_TCB_t *task = list_pop_sl(&pending_list);
		// Unset the yield bit
		task->state &= ~TASK_STATE_YIELD;
		// Add back into task list
		list_add(&task_list, task);
	}
	// If tasks in sleep list and there is a task which should wake up
	while (sleep_list.head && sleep_list.head->wakeup_time < OS_elapsedTicks()) {
		// Remove top awake task
		OS_TCB_t *task = list_pop_sl(&sleep_list);
		// Unset sleep bit
		task->state &= ~TASK_STATE_SLEEP;
		list_add(&task_list, task);
		// Move to next task in sleep list
		sleep_list.head = sleep_list.head->next;
	}
	
	// If there is a task in the list
	if (task_list.head) {
		OS_TCB_t const * const initial_task = task_list.head;
		OS_TCB_t *current_task = task_list.head;
		
		// Previous task's priority is higher than current task
		if (current_task->prev->priority < current_task->priority) {
			// Loop round to first highest priority
			do {
				// Move to next task
				current_task = current_task->next;
				// Check if previous task has a lower priority
				if (current_task->prev->priority > current_task->priority) {
					// First highest priority task found, update head
					task_list.head = current_task;
					// Return highest priority task to run now
					return task_list.head;
				}
			} while (current_task != initial_task);
		}
		else {
			// Next task is the same priority so round robin scenario
			if (current_task->next->priority == current_task->priority) {
				// Move to next task
				current_task = current_task->next;
				// Update head
				task_list.head = current_task;
				// Return next task with same priority to run now
				return current_task;
			}
			else {
				// Next task is not same priority, and no higher priority in list, so loop round to first same priority task
				do {
					current_task = current_task->next;
					// Same priority task found
					if (current_task->priority == initial_task->priority) {
						// Update head
						task_list.head = current_task;
						// Return first task with same priority to run now
						return current_task;
					}
				} while (current_task != initial_task);
			}
		}
		// No conditions met, so return same task
		return current_task;
	}
	// No tasks are runnable, so return the idle task
	return _OS_idleTCB_p;
}

/* Initialises a task control block (TCB) and its associated stack.  See os.h for details. */
uint32_t OS_initialiseTCB(OS_TCB_t * TCB, uint32_t * const stack, void (* const func)(void const * const), void const * const data, uint32_t const priority) {
	TCB->sp = stack - (sizeof(_OS_StackFrame_t) / sizeof(uint32_t));
	TCB->state = 0;
	TCB->wakeup_time = 0;
	if (priority > MAX_NUM_PRIORITY_LEVELS) {
		TCB->priority = MAX_NUM_PRIORITY_LEVELS;
	}
	else if (priority < 1) {
		return 0;
	}
	else {
		TCB->priority = priority;
	}
	TCB->prev = TCB->next = 0;
	_OS_StackFrame_t *sf = (_OS_StackFrame_t *)(TCB->sp);
	/* By placing the address of the task function in pc, and the address of _OS_task_end() in lr, the task
	   function will be executed on the first context switch, and if it ever exits, _OS_task_end() will be
	   called automatically */
	*sf = (_OS_StackFrame_t) {
		.r0 = (uint32_t)(data),
		.r1 = 0,
		.r2 = 0,
		.r3 = 0,
		.r4 = 0,
		.r5 = 0,
		.r6 = 0,
		.r7 = 0,
		.r8 = 0,
		.r9 = 0,
		.r10 = 0,
		.r11 = 0,
		.r12 = 0,
		.lr = (uint32_t)_OS_task_end,
		.pc = (uint32_t)(func),
		.psr = xPSR_T_Msk  /* Sets the thumb bit to avoid a big steaming fault */
	};
	
	return 1;
}

/* 'Add task' */
void OS_addTask(OS_TCB_t * const tcb) {
	list_add(&task_list, tcb);
}

/* SVC handler that's called by _OS_task_end when a task finishes.  Removes the
   task from the scheduler and then queues PendSV to reschedule. */
void _OS_taskExit_delegate(void) {
	// Remove the given TCB from the list of tasks so it won't be run again
	OS_TCB_t * tcb = OS_currentTCB();
	list_remove(&task_list, tcb);
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* Return the notification counter value */
uint32_t get_notification_counter(void) {
	return notification_counter;
}