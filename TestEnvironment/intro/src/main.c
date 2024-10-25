#include "Utils/utils.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static uint32_t fib(uint32_t fib_index) {
	uint32_t fib_value = 0;
	uint32_t fib_value_last = 0;
	uint32_t fib_value_last_last = 0;
	
	if (fib_index <= 2) {
		fib_value = 1;
		goto next;
	}
	fib_index = fib_index - 1;
	fib_value_last = fib(fib_index);
	next:
		fib_value_last_last = fib(fib_index);
		fib_value = fib_value_last + fib_value_last_last;
	
	return fib_value;
}


int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("Test Program\r\n");
	
	uint32_t fib_index = 7;
	
	printf("Fib at index: %" PRIu32 " equals --> %" PRIu32 "\r\n", fib_index, fib(fib_index));
	
	
	
	// Infinite loop: embedded software must never end
	while(1) { }	
}
