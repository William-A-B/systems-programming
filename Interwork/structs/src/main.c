#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
	void *ptr;
	int32_t y;
	void (*callback) (void *);
	int32_t x;
} structure_t;

#pragma pack(pop)

void process(structure_t * s);

void printInteger(void *ptr) {
	int32_t *value = ptr;
	printf("%" PRId32 "\r\n", *value);
}

void printDouble(void *ptr) {
	double *value = ptr;
	printf("%f\r\n", *value); 
}

int main(void) {
	configClock();
	configUSART2(38400);

	structure_t structure = {.x = 5, .y = 10};
	
	int32_t val = 3;
	
	structure.ptr = &val;
	
	structure.callback = printInteger;
	
	process(&structure);
	
	structure.callback(&structure.x);
	structure.callback(&structure.y);
	
	structure_t my_struct = {.x = 3, .y = 6};
	
	double a_double = 5.5f;
	
	my_struct.ptr = &a_double;
	
	my_struct.callback = printDouble;
	
	process(&my_struct);
	
	structure.callback(&my_struct.x);
	structure.callback(&my_struct.y);
	
	while(1);
}
