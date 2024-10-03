#include "Utils/utils.h"
#include <stdio.h>

void square(long long * value);

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	int i = 3;
	
	printf("i = %p\r\n", (void *)&i);
	printf("i + 1 = %p\r\n\n", ((&i)+1));
	
	long long j = 3;
	
	printf("j = %lld\r\n", j);
	
	square(&j);
	
	printf("j squared = %lld\r\n", j);
	
	
	while(1);

}

void square(long long * value) {
	*value = (*value)*(*value);
}