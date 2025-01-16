#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "OS/os.h"
#include "Utils/utils.h"
#include "Utils/mode_utils.h"
#include "OS/mutex.h"
#include "OS/semaphore.h"
#include "Memory/mempool.h"
#include "Memory/queue.h"

static OS_mutex_t mutex = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t pool_mutex = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t queue_mutex = OS_MUTEX_STATIC_INITIALISER;
static queue_t queue = QUEUE_INITIALISER;
static mempool_t pool = MEMPOOL_INITIALISER;
//static OS_semaphore_t sem = OS_SEMAPHORE_STATIC_INITIALISER(1);

/* Example data packet structure, just for demonstration purposes */
typedef struct {
	uint32_t id;
	char data[16];
} packet_t;

static void task1(void const *const args) {
	(void) args;
	// Initialise task name data
	packet_t *task_name = pool_allocate(&pool, &pool_mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 1", 15);
	// Initialise task priority data
	packet_t *task_priority = pool_allocate(&pool, &pool_mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 1", 15);
	
	// Allocate data packet and add to queue for another task to use
	packet_t *data_packet_1 = pool_allocate(&pool, &pool_mutex);
	packet_t *data_packet_2 = pool_allocate(&pool, &pool_mutex);
	
	data_packet_1->id = 1;
	data_packet_2->id = 2;
	
	strncpy(data_packet_1->data, "Number 1", 15);
	strncpy(data_packet_2->data, "Number 2", 15);
	
	// Store data on queue
	queue_put(&queue, (uint32_t *) data_packet_1, &queue_mutex);
	queue_put(&queue, (uint32_t *) data_packet_2, &queue_mutex);
	
	// Display data
	for (uint32_t i = 0; i < 6; i++) {
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

static void task2(void const *const args) {
	(void) args;
	// Initialise task name data
	packet_t *task_name = pool_allocate(&pool, &pool_mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 2", 15);
	// Initialise task priority data
	packet_t *task_priority = pool_allocate(&pool, &pool_mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 1", 15);

	// Display data
	for (uint32_t i = 0; i < 6; i++) {
//		if (i == 3) {
//			OS_sleep(100);
//		}
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

static void task3(void const *const args) {
	(void) args;
	// Initialise task name data
	packet_t *task_name = pool_allocate(&pool, &pool_mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 3", 15);
	// Initialise task priority data
	packet_t *task_priority = pool_allocate(&pool, &pool_mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 2", 15);
	
	// Retrieve data packet from another task
	packet_t *data_packet = 0;
	queue_get(&queue, (uint32_t **) &data_packet, &queue_mutex);
	
	// Print data packet
	OS_mutex_acquire(&mutex);
	printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
	OS_mutex_release(&mutex);
	
	pool_deallocate(&pool, data_packet, &pool_mutex);
	
	// Display data
	for (uint32_t i = 0; i < 6; i++) {
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

static void task4(void const *const args) {
	(void) args;
	// Initialise task name data
	packet_t *task_name = pool_allocate(&pool, &pool_mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 4", 15);
	// Initialise task priority data
	packet_t *task_priority = pool_allocate(&pool, &pool_mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 3", 15);
	
	// Retrieve data packet from another task
	packet_t *data_packet = 0;
	queue_get(&queue, (uint32_t **) &data_packet, &queue_mutex);
	
	// Print data packet
	OS_mutex_acquire(&mutex);
	printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
	OS_mutex_release(&mutex);
	
	pool_deallocate(&pool, data_packet, &pool_mutex);
	
	// Display task data
	for (uint32_t i = 0; i < 6; i++) {
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

static void task5(void const *const args) {
	(void) args;
	
	for (uint32_t i = 0; i < 10000; i++) {
		OS_mutex_acquire(&mutex);
		printf("Task 5\r\n");
		OS_mutex_release(&mutex);
	}
}

static void task6(void const *const args) {
	(void) args;
	
	for (uint32_t i = 0; i < 10000; i++) {
		OS_mutex_acquire(&mutex);
		printf("Task 66\r\n");
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
	static uint32_t stack5[128] __attribute__ (( aligned(8) ));
	static uint32_t stack6[128] __attribute__ (( aligned(8) ));
	static OS_TCB_t TCB1, TCB2, TCB3, TCB4;
	static OS_TCB_t TCB5, TCB6;
	
	/* Initialise the TCBs using the two functions above */
	success = success & OS_initialiseTCB(&TCB1, stack1+128, task1, NULL, 1);
	success = success & OS_initialiseTCB(&TCB2, stack2+128, task2, NULL, 1);
	success = success & OS_initialiseTCB(&TCB3, stack3+128, task3, NULL, 2);
	success = success & OS_initialiseTCB(&TCB4, stack4+128, task4, NULL, 3);
	success = success & OS_initialiseTCB(&TCB5, stack5+128, task5, NULL, 10);
	success = success & OS_initialiseTCB(&TCB6, stack6+128, task6, NULL, 10);
	
	if (!success) {
		printf("One or more tasks initialised incorrectly\r\n");
		return 0;
	}
	
	/* Add the tasks to the scheduler */
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	OS_addTask(&TCB4);
//	OS_addTask(&TCB5);
//	OS_addTask(&TCB6);
	
	/* Declare and intialise a memory pool */
	pool_init(&pool, 16, 20, &mutex);

	/* Start the OS */
	OS_start();
	
}
