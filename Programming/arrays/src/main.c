#include "Utils/utils.h"
#include <stdio.h>

void processArray(int *array);

int main(void) {
	
	configClock();
	configUSART2(38400);

	int numbers[] = {1,3,5,7,9};

	*numbers = 11;
	*(numbers + 3) = 42;
	
	int *pointer = numbers;
	

	for (int i = 0; i < 5; ++i) {
		printf("numbers[%d] = %d\n", i, numbers[i]);
	}
	
	for (int i = 0; i < 5; ++i) {
		printf("pointer[%d] = %d\r\n", i, pointer[i]);
	}
	
	int numbers_size = sizeof(numbers);
	int pointer_size = sizeof(pointer);
	
	printf("numbers size = %d\r\n", numbers_size);
	printf("pointer size = %d\r\n\n", pointer_size);
	
	printf("numbers[%d] = %d\r\n", 2, numbers[2]);
	
	processArray(numbers);
	
	printf("numbers[%d] = %d\r\n", 2, numbers[2]);
	
	while(1);

}

void processArray(int *array) {
	array[2] = 10;
}
