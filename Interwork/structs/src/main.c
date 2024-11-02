#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
	int32_t x;
	int32_t y;
	void *ptr;
	void (*callback) (void *);
} structure_t;

#pragma pack(pop)

void process(structure_t * s);

void printInteger(void *ptr) {
	int32_t *value = ptr;
	printf("%" PRId32 "\r\n", *value);
}

int main(void) {
	configClock();
	configUSART2(38400);

	
	structure_t structure = {.x = 5, .y = 10};
	
	int32_t val = 3;
	
	structure.ptr = &val;
	
	structure.callback = printInteger;
	
	process(&structure);
	structure.callback((void*) structure.x);
	structure.callback((void*) structure.y);
	
	

	while(1);
}
