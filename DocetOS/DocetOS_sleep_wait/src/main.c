#include "OS/os.h"
#include "Utils/utils.h"
#include <stdio.h>
#include "Utils/mode_utils.h"
#include <inttypes.h>
#include "OS/mutex.h"

static OS_mutex_t mutex = OS_MUTEX_STATIC_INITIALISER;

static void task1(void const *const args) {
	(void) args;
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("AAAAA");
		OS_mutex_release(&mutex);
	}
}

static void task2(void const *const args) {
	(void) args;
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("BBBBB");
		OS_mutex_release(&mutex);
		//OS_sleep(50);
	}
}

static void task3(void const *const args) {
	(void) args;
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("CCCCC");
		OS_mutex_release(&mutex);
	}
}

static void task4(void const *const args) {
	(void) args;
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("DDDDD");
		OS_mutex_release(&mutex);
	}
}

/* MAIN FUNCTION */

int main(void) {
	
	configClock();
	configUSART2(38400);
		
	printf("\r\nDocetOS\r\n");
	
	uint32_t success = 1;
	
	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	static uint32_t stack1[128] __attribute__ (( aligned(8) ));
	static uint32_t stack2[128] __attribute__ (( aligned(8) ));
	static uint32_t stack3[128] __attribute__ (( aligned(8) ));
	static uint32_t stack4[128] __attribute__ (( aligned(8) ));
	static OS_TCB_t TCB1, TCB2, TCB3, TCB4;
	
	/* Initialise the TCBs using the two functions above */
	success = success & OS_initialiseTCB(&TCB1, stack1+128, task1, NULL, 2);
	success = success & OS_initialiseTCB(&TCB2, stack2+128, task2, NULL, 3);
	success = success & OS_initialiseTCB(&TCB3, stack3+128, task3, NULL, 4);
	success = success & OS_initialiseTCB(&TCB4, stack4+128, task4, NULL, 5);
	
	if (!success) {
		printf("One or more tasks initialised incorrectly\r\n");
		return 0;
	}
	
	/* Add the tasks to the scheduler */
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
//	OS_addTask(&TCB4);

	/* Start the OS */
	OS_start();
	
}
