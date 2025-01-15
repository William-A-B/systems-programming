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
static queue_t queue = QUEUE_INITIALISER;
//static OS_semaphore_t sem = OS_SEMAPHORE_STATIC_INITIALISER(1);

/* Example data packet structure, just for demonstration purposes */
typedef struct {
	uint32_t id;
	char data[16];
} packet_t;

static void task1(void const *const args) {
	(void) args;
	/* Declare and intialise a memory pool */
	static mempool_t pool = MEMPOOL_INITIALISER;
	pool_init(&pool, 16, 10, &mutex);
	// Initialise some example data
	packet_t *task_name = pool_allocate(&pool, &mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 1", 15);
	packet_t *task_priority = pool_allocate(&pool, &mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 1", 15);
	
	
	packet_t *data_packet = pool_allocate(&pool, &mutex);
	data_packet->id = 12345;
	strncpy(data_packet->data, "Hello World", 15);
	
	// Store data on queue
	queue_put(&queue, (uint32_t *) task_name, &mutex);
	
	
	
	
	// Display data
	for (uint32_t i = 0; i < 4; i++) {
		OS_mutex_acquire(&mutex);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_name->id, task_name->data);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_priority->id, task_priority->data);
		OS_mutex_release(&mutex);
	}
	pool_deallocate(&pool, task_name, &mutex);
	pool_deallocate(&pool, task_priority, &mutex);
}

static void task2(void const *const args) {
	(void) args;
	/* Declare and intialise a memory pool */
	static mempool_t pool = MEMPOOL_INITIALISER;
	pool_init(&pool, 16, 10, &mutex);
	// Initialise some example data
	packet_t *task_name = pool_allocate(&pool, &mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 2", 15);
	packet_t *task_priority = pool_allocate(&pool, &mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 1", 15);


	// Display data
	for (uint32_t i = 0; i < 4; i++) {
		if (i == 2) {
			OS_sleep(100);
		}
		OS_mutex_acquire(&mutex);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_name->id, task_name->data);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_priority->id, task_priority->data);
		OS_mutex_release(&mutex);
	}
	pool_deallocate(&pool, task_name, &mutex);
	pool_deallocate(&pool, task_priority, &mutex);
}

static void task3(void const *const args) {
	(void) args;
	/* Declare and intialise a memory pool */
	static mempool_t pool = MEMPOOL_INITIALISER;
	pool_init(&pool, 16, 10, &mutex);
	// Initialise some example data
	packet_t *task_name = pool_allocate(&pool, &mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 3", 15);
	packet_t *task_priority = pool_allocate(&pool, &mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 2", 15);
	
	
	packet_t *data_packet = 0;
	queue_get(&queue, (uint32_t **) data_packet, &mutex);
	
	OS_mutex_acquire(&mutex);
	printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
	OS_mutex_release(&mutex);
	
	// Display data
	for (uint32_t i = 0; i < 4; i++) {
		OS_mutex_acquire(&mutex);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_name->id, task_name->data);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_priority->id, task_priority->data);
		OS_mutex_release(&mutex);
	}
	pool_deallocate(&pool, task_name, &mutex);
	pool_deallocate(&pool, task_priority, &mutex);
}

static void task4(void const *const args) {
	(void) args;
	/* Declare and intialise a memory pool */
	static mempool_t pool = MEMPOOL_INITIALISER;
	pool_init(&pool, 16, 10, &mutex);
	// Initialise some example data
	packet_t *task_name = pool_allocate(&pool, &mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 4", 15);
	packet_t *task_priority = pool_allocate(&pool, &mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 3", 15);
	
	
	packet_t *data_packet = 0;
	queue_get(&queue, (uint32_t **) data_packet, &mutex);
	
	OS_mutex_acquire(&mutex);
	printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
	OS_mutex_release(&mutex);
	
	// Display data
	for (uint32_t i = 0; i < 4; i++) {
		OS_mutex_acquire(&mutex);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_name->id, task_name->data);
		printf("ID %" PRIu32 ", Data: '%s'\r\n", task_priority->id, task_priority->data);
		OS_mutex_release(&mutex);
	}
	pool_deallocate(&pool, task_name, &mutex);
	pool_deallocate(&pool, task_priority, &mutex);
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
	success = success & OS_initialiseTCB(&TCB1, stack1+128, task1, NULL, 1);
	success = success & OS_initialiseTCB(&TCB2, stack2+128, task2, NULL, 1);
	success = success & OS_initialiseTCB(&TCB3, stack3+128, task3, NULL, 2);
	success = success & OS_initialiseTCB(&TCB4, stack4+128, task4, NULL, 3);
	
	if (!success) {
		printf("One or more tasks initialised incorrectly\r\n");
		return 0;
	}
	
	/* Add the tasks to the scheduler */
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	OS_addTask(&TCB4);

	/* Start the OS */
	OS_start();
	
}
