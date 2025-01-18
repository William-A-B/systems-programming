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

// Mutexes for mutual exclusion in tasks
static OS_mutex_t mutex = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t pool_mutex = OS_MUTEX_STATIC_INITIALISER;
static OS_mutex_t queue_mutex = OS_MUTEX_STATIC_INITIALISER;
// Pointer queue for task communication
static queue_t queue = QUEUE_INITIALISER;
// Memory pool for allocating storage
static mempool_t pool = MEMPOOL_INITIALISER;

/**
 * @brief Example data structure for storing user data in
 */
typedef struct {
	uint32_t id;
	char data[16];
} packet_t;

/**
 * @brief Task 1 is the first task, which creates two data packets
 * to add into the queue to be read by other tasks.
 * 
 * @param args - Parameter for passing arguments across to task
 */
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

/**
 * @brief Task 2 demonstrates sleeping halfway through the number of iterations
 * 
 * @param args - Parameter for passing arguments across to task
 */
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
		if (i == 3) {
			OS_sleep(100);
		}
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

/**
 * @brief Task 3 is the first task with equal priority to the first two
 * It gets the first data off the queue and displays it
 * 
 * @param args - Parameter for passing arguments across to task
 */
static void task3(void const *const args) {
	(void) args;
	// Initialise task name data
	packet_t *task_name = pool_allocate(&pool, &pool_mutex);
	task_name->id = 1;
	strncpy(task_name->data, "Task 3", 15);
	// Initialise task priority data
	packet_t *task_priority = pool_allocate(&pool, &pool_mutex);
	task_priority->id = 2;
	strncpy(task_priority->data, "Priority 1", 15);
	
	// Retrieve data packet from another task
	packet_t *data_packet = 0;
	queue_get(&queue, (uint32_t **) &data_packet, &queue_mutex);
	
	// Display data
	for (uint32_t i = 0; i < 6; i++) {
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		// Print data packet once
		if (i == 0) {
			printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
			OS_mutex_release(&mutex);
			OS_sleep(1000);
			OS_mutex_acquire(&mutex);
		}
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, data_packet, &pool_mutex);
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

/**
 * @brief Task 4 is the first task with a lower priority than the first three
 * It isn't called until task 1, 2, & 3 have finished (unless one of them is waiting or sleeping)
 * It gets the second data packet off the queue and displays it
 * 
 * @param args - Parameter for passing arguments across to task
 */
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
	
	// Display task data
	for (uint32_t i = 0; i < 6; i++) {
		OS_mutex_acquire(&mutex);
		printf("%s, ", task_name->data);
		printf("%s\r\n", task_priority->data);
		// Print data packet once
		if (i == 0) {
			printf("ID %" PRIu32 ", Data: '%s'\r\n", data_packet->id, data_packet->data);
		}
		OS_mutex_release(&mutex);
	}
	// Free memory back to the pool
	pool_deallocate(&pool, data_packet, &pool_mutex);
	pool_deallocate(&pool, task_name, &pool_mutex);
	pool_deallocate(&pool, task_priority, &pool_mutex);
}

/**
 * @brief Task 5 is one of two lowest priority tasks in the demonstration
 * It cycles in a round robin fashion with Task 6.
 * Task 5 initialises a counter variable to 0, and allocates space on the memory pool for it
 * Task 5 increments the counter variable to odd numbers, and then stores the variable on the 
 * pointer queue ready for Task 6 to receive it
 * 
 * @param args - Parameter for passing arguments across to task
 */
static void task5(void const *const args) {
	(void) args;
	uint32_t *counter = pool_allocate(&pool, &pool_mutex);;
	(*counter) = 0;
	queue_put(&queue, (uint32_t *) counter, &queue_mutex);
//	for (uint32_t i = 0; i < 30; i++) {
//		OS_mutex_acquire(&mutex);
//		printf("Task 5\r\n");
//		OS_mutex_release(&mutex);
//		// Retrieve counter value
//		queue_get(&queue, (uint32_t **) &counter, &queue_mutex);
//		// Increment counter value
//		(*counter)++;
//		// Print counter value
//		OS_mutex_acquire(&mutex);
//		printf("Counter = %" PRIu32 "\r\n", *counter);
//		OS_mutex_release(&mutex);
//		// Store counter value on queue
//		queue_put(&queue, (uint32_t *) counter, &queue_mutex);
//	}
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("Task 5\r\n");
		OS_mutex_release(&mutex);
		// Retrieve counter value
		queue_get(&queue, (uint32_t **) &counter, &queue_mutex);
		// Increment counter value
		(*counter)++;
		// Print counter value
		OS_mutex_acquire(&mutex);
		printf("Counter = %" PRIu32 "\r\n", *counter);
		OS_mutex_release(&mutex);
		// Store counter value on queue
		queue_put(&queue, (uint32_t *) counter, &queue_mutex);
	}
	pool_deallocate(&pool, counter, &pool_mutex);
}

/**
 * @brief Task 6 cycles in a round robin with Task 5 as they both have the same priority
 * Task 6 retrieves the latest value of counter from the pointer queue
 * It then increments it to an even number before storing it back on the queue for Task 5
 * 
 * @param args - Parameter for passing arguments across to task
 */
static void task6(void const *const args) {
	(void) args;
	uint32_t *counter = pool_allocate(&pool, &pool_mutex);
//	for (uint32_t i = 0; i < 30; i++) {
//		OS_mutex_acquire(&mutex);
//		printf("Task 6\r\n");
//		OS_mutex_release(&mutex);
//		// Retrieve counter value
//		queue_get(&queue, (uint32_t **) &counter, &queue_mutex);
//		// Increment counter value
//		(*counter)++;
//		// Print counter value
//		OS_mutex_acquire(&mutex);
//		printf("Counter = %" PRIu32 "\r\n", *counter);
//		OS_mutex_release(&mutex);
//		if (i < 29) {
//			// Store counter value on queue apart from when finished
//			queue_put(&queue, (uint32_t *) counter, &queue_mutex);
//		}
//	}
	while (1) {
		OS_mutex_acquire(&mutex);
		printf("Task 6\r\n");
		OS_mutex_release(&mutex);
		// Retrieve counter value
		queue_get(&queue, (uint32_t **) &counter, &queue_mutex);
		// Increment counter value
		(*counter)++;
		// Print counter value
		OS_mutex_acquire(&mutex);
		printf("Counter = %" PRIu32 "\r\n", *counter);
		OS_mutex_release(&mutex);
		// Store counter value on queue
		queue_put(&queue, (uint32_t *) counter, &queue_mutex);
	}
	
	pool_deallocate(&pool, counter, &pool_mutex);
}

/**
 * @brief MAIN FUNCTION
 * Initialises all the tasks and memory required for tasks 
 * and sets them up within the operating system
 * Then starts the OS running
 */
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
	success = success & OS_initialiseTCB(&TCB3, stack3+128, task3, NULL, 1);
	success = success & OS_initialiseTCB(&TCB4, stack4+128, task4, NULL, 2);
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
	OS_addTask(&TCB5);
	OS_addTask(&TCB6);
	
	/* Declare and intialise a memory pool */
	pool_init(&pool, 16, 20);

	/* Start the OS */
	OS_start();
	
}
