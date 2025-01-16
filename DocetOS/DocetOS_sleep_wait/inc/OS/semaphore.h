#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

typedef struct {
	uint32_t initial_tokens;
	volatile uint32_t tokens;
} OS_semaphore_t;

#define OS_SEMAPHORE_STATIC_INITIALISER(initial_tokens, tokens) { initial_tokens, tokens }

/* Initialise a semaphore with a given number of tokens */
void OS_semaphore_init(OS_semaphore_t *sem, uint32_t initial_tokens);

/* Obtain a token from the semaphore container */
void OS_semaphore_obtain(OS_semaphore_t *sem);

/* Release a token from the semaphore container */
void OS_semaphore_release(OS_semaphore_t *sem);

#endif /* SEMAPHORE_H */