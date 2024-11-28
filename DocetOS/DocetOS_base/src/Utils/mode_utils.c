#define OS_INTERNAL
#include "/Utils/mode_utils.h"
#include <stdio.h>
#include <inttypes.h>
#include "OS/os.h"


void check_status_delegate(_OS_SVC_StackFrame_t *);

void reportState(void) {
	
	uint32_t PSR = getPSR();
	uint32_t CONTROL = getCONTROL();
	uint32_t PSR_mask = (1 << 9) - 1;
	uint32_t nPRIV_position = 0;
	uint32_t SPSEL_position = 1;
	
	
	if ((PSR & PSR_mask) == 0) {
		printf("Thread Mode ");
		
		if ((CONTROL >> nPRIV_position) == 0) {
			printf("Privileged ");
		}
		else {
			printf("Unprivileged ");
		}
	}
	else {
		printf("Handler Mode Privileged ");
	}
	
	if ((CONTROL >> SPSEL_position) == 0) {
		printf("MSP in use\r\n");
	}
	else {
		printf("PSP in use\r\n");
	}
	
}

void check_status_delegate(_OS_SVC_StackFrame_t * const stack) {
	reportState();
	
	printf("Value = %"PRIu32"\r\n", stack->r0);
	stack->r0 = stack->r0 << 1;
}