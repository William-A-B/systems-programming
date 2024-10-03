#include "count.h"
#include <stdio.h>

unsigned int counter;

void count(void) {
	printf("count(): Counter increased from %d", counter);
	counter++;
	printf(" to %d\r\n", counter);
}
