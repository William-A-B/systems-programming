#include "OS/semaphore.h"
#include <stdint.h>
#include "OS/os.h"

/* Initialise a semaphore with a given number of tokens */
void OS_semaphore_init(OS_semaphore_t *sem, uint32_t initial_tokens) {
	sem->tokens = initial_tokens;
	sem->initial_tokens = initial_tokens;
}

/* Obtain a token from the semaphore container */
void OS_semaphore_obtain(OS_semaphore_t *sem) {
	uint32_t failure = 1;
	do {
		// Get current token count
		uint32_t current_token_count = (uint32_t) __LDREXW ((uint32_t *)&(sem->tokens));
		// No tokens available, clear exclusive lock and block.
		if (current_token_count == 0) {
			__CLREX();
		}
		// Decrement token count by one, as successfully obtained token
		failure = __STREXW ((current_token_count - 1), (uint32_t *)&(sem->tokens));
	} while(failure);
}

/* Release a token from the semaphore container */
void OS_semaphore_release(OS_semaphore_t *sem) {
	uint32_t failure = 1;
	do {
		// Get current token count
		uint32_t current_token_count = (uint32_t) __LDREXW ((uint32_t *)&(sem->tokens));
		if (current_token_count == sem->initial_tokens) {
			__CLREX();
		}
		// Increment token count by one, as successfully released token
		failure = __STREXW ((current_token_count + 1), (uint32_t *)&(sem->tokens));
	} while(failure);
}