#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

typedef struct {
	// Record initial number of tokens so container doesn't overflow
	uint32_t initial_tokens;
	// Number of available tokens in the semaphore container
	volatile uint32_t tokens;
} OS_semaphore_t;

#define OS_SEMAPHORE_STATIC_INITIALISER(initial_tokens, tokens) { initial_tokens, tokens }

/**
 * @brief Non Static Initialisation of Semaphore
 * Allows for the initialisation of a semaphore with a given number of tokens
 * 
 * @param sem - Semaphore to be initialised
 * @param initial_tokens - Number of tokens to initialise the semaphore with
 */
void OS_semaphore_init(OS_semaphore_t *sem, uint32_t initial_tokens);

/**
 * @brief Obtain a token from the semaphore container
 * 
 * @param sem - Semaphore to obtain token from
 */
void OS_semaphore_obtain(OS_semaphore_t *sem);

/**
 * @brief Release a token from the semaphore container
 * 
 * @param sem - Semaphore to release token from
 */
void OS_semaphore_release(OS_semaphore_t *sem);

#endif /* SEMAPHORE_H */