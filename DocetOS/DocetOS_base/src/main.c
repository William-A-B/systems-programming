#include "OS/os.h"
#include "Utils/utils.h"
#include <stdio.h>
#include "Utils/mode_utils.h"
#include <inttypes.h>

static void task1(void const *const args) {
	(void) args;
	for (uint_fast16_t i = 0; i < 1000; ++i) {
		printf("AAAAAAAA");
	}
}

static void task2(void const *const args) {
	(void) args;
	while (1) {
		printf("BBBBBBBB");
	}
}

static void task3(void const *const args) {
	(void) args;
//	while (1) {	
		reportState();
		uint32_t return_val = check_CPU_state(5);
		printf("returned value = %"PRIu32"\r\n", return_val);
//	}
}

/* MAIN FUNCTION */

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("\r\nDocetOS\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	static uint32_t stack1[128] __attribute__ (( aligned(8) ));
	static uint32_t stack2[128] __attribute__ (( aligned(8) ));
	static uint32_t stack3[128] __attribute__ (( aligned(8) ));
	static OS_TCB_t TCB1, TCB2, TCB3;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+128, task1, NULL);
	OS_initialiseTCB(&TCB2, stack2+128, task2, NULL);
	OS_initialiseTCB(&TCB3, stack3+128, task3, NULL);
	
	/* Add the tasks to the scheduler */
//	OS_addTask(&TCB1);
//	OS_addTask(&TCB2);
	OS_addTask(&TCB3);

	printf("PSR = %08x\r\n", getPSR());
	printf("PSR = %08x\r\n", getCONTROL());
	reportState();

	/* Start the OS */
	OS_start();
	
}
