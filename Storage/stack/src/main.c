#include "Utils/utils.h"
#include <stdio.h>
#include "stack.h"
#define STACK_SIZE 10

int main(void) {
	
	configClock();
	configUSART2(38400);
		
	// Stack pointer, initially points to the first array element
	int32_t *stack_p;
	// Array to hold stack entries
	int32_t stack[10];
	
	stack_p = stack;
	
	push(&stack_p, 5);
	printf("Popped value: %d\r\n", pop(&stack_p));
	
	for (int32_t i = 0; i < 30; i++) {
		push(&stack_p, i);
	}
	
	for (int32_t i = 0; i < 30; i++) {
		printf("Popped value: %d\r\n", pop(&stack_p));
	}
	

	while(1);
	// Finish this!
}
