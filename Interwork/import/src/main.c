#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

uint32_t fib(uint32_t n);
void report(uint32_t value);

//int32_t calculate(int32_t x, int32_t y);

void report(uint32_t value) {
	printf("%" PRIu32 "\r\n", value);
}

int main(void) {
	configClock();
	configUSART2(38400);
	
//	int32_t result = calculate(5, 2);
	
	uint32_t result = 0;

	result = fib(8);
	
	uint32_t result_copy = result;

	printf("Result = %d\r\n", result);

	while(1);
}
