#define OS_INTERNAL

#include "OS/scheduler.h"
#include "OS/os.h"

#include "stm32f4xx.h"

#include <string.h>

void _OS_wait_delegate(_OS_SVC_StackFrame_t *);

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

static _OS_tasklist_t task_list = {.head = 0};
static _OS_tasklist_t wait_list = {.head = 0};
static _OS_tasklist_t pending_list = {.head = 0};

static void list_add(_OS_tasklist_t *list, OS_TCB_t *task) {
	if (!(list->head)) {
		task->next = task;
		task->prev = task;
		list->head = task;
	} else {
		task->next = list->head;
		task->prev = list->head->prev;
		task->prev->next = task;
		list->head->prev = task;
	}
}

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

/* Remove current task from task list and place it in waiting list */
void _OS_wait_delegate(_OS_SVC_StackFrame_t * const stack) {
	if (stack->r0 != notification_counter) {
		return;
	}
	
	OS_TCB_t *current_task = OS_currentTCB();
	list_remove(&task_list, current_task);
	list_push_sl(&wait_list, current_task);
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* Notify all tasks in wait list and move them to pending list */
void OS_notifyAll(void) {
	// While wait list still has tasks in it
	// Remove all tasks and add them to the pending list
	OS_TCB_t *task;
	notification_counter++;
	while ((task = list_pop_sl(&wait_list))) {
		list_push_sl(&pending_list, task);
	}
}

/* Round-robin scheduler */
OS_TCB_t const * _OS_schedule(void) {

	// While pending list is not empty, remove them and add them into the round robin
	while (pending_list.head != NULL) {
		OS_TCB_t *task = list_pop_sl(&pending_list);
		list_add(&task_list, task);
	}
	// If there is a task in the list
	if (task_list.head) {
		// Store current head to compare with loop
		OS_TCB_t const * const currentHead = task_list.head;
		do {
			// Get next task
			task_list.head = task_list.head->next;
			// If task is not asleep, unset the yield bit, and return new task
			if (!(task_list.head->state & TASK_STATE_SLEEP)) {
				task_list.head->state &= ~TASK_STATE_YIELD;
				return task_list.head;
			}
			// If task wakup time is less than the curernt time, then wake up task and unset the yield and sleep bit
			// Return new task
			if (task_list.head->wakeup_time < OS_elapsedTicks()) {
				task_list.head->state &= ~(TASK_STATE_SLEEP | TASK_STATE_YIELD);
				return task_list.head;
			}
		} while (task_list.head != currentHead);
		
	}
	// No tasks are runnable, so return the idle task
	return _OS_idleTCB_p;
}

/* Initialises a task control block (TCB) and its associated stack.  See os.h for details. */
void OS_initialiseTCB(OS_TCB_t * TCB, uint32_t * const stack, void (* const func)(void const * const), void const * const data) {
	TCB->sp = stack - (sizeof(_OS_StackFrame_t) / sizeof(uint32_t));
	TCB->state = 0;
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

uint32_t get_notification_counter(void) {
	return notification_counter;
}
