#include "Utils/utils.h"
#include <stdio.h>

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	printf("char: %d\r\n", sizeof(char));
	
	int x = 42000;
	short y = 42;
	
	y = (short) x;

	printf("x = %d\r\n", x);
	printf("y = %hd\r\n", y);
	
	unsigned int mask = (0xBAUL) << 24;
	printf("mask: 0x%08X\r\n", mask);

	
	(void) y;


	while(1);
}
