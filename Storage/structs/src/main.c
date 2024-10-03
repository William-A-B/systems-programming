#include "Utils/utils.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

//#pragma pack(push)
//#pragma pack(1)

typedef struct {
	int8_t x;
	uint8_t _padding[3];
	int32_t y;
} test_t;

//#pragma pack(pop)

int main(void) {
	
	configClock();
	configUSART2(38400);
		
	test_t test  = {.x = 100, .y = -1};
	test_t *test_p = &test;
	
	printf("Test size = %d\r\n", sizeof(test));
	printf("Test_t size = %d\r\n", sizeof(test_t));

	printf("test.x = %" PRId8 "\r\n", test.x);
	printf("test.y = %" PRId32 "\r\n", test.y);
	
	test = (test_t) {.x = 12, .y = 432};
	
	printf("test.x = %" PRId8 "\r\n", test.x);
	printf("test.y = %" PRId32 "\r\n", test.y);
	
	test_p->x = 5;
	test_p->y = 20;
	
	printf("test_p->x = %" PRId8 "\r\n", test_p->x);
	printf("test_p->y = %" PRId32 "\r\n", test_p->y);
	
	printf("test.x = %" PRId8 "\r\n", test.x);
	printf("test.y = %" PRId32 "\r\n", test.y);
	
	
	
	while(1);
}
