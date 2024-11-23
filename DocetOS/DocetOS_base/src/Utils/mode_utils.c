#include "/Utils/mode_utils.h"
#include <stdio.h>

void check_status_delegate();

void reportState() {
	
	uint32_t PSR = getPSR();
	uint32_t CONTROL = getCONTROL();
	uint32_t PSR_mask = (1 << 9) - 1;
	uint32_t nPRIV_position = 0;
	uint32_t SPSEL_position = 1 << 1;
	
	
	if ((PSR & PSR_mask) == 0) {
		printf("Thread Mode");
		
		if ((CONTROL >> nPRIV_position) == 0) {
			printf("Privileged");
		}
		else {
			printf("Unprivileged");
		}
	}
	else {
		printf("Handler Mode");
	}
	
	if ((CONTROL >> SPSEL_position) == 0) {
		printf("MSP in use");
	}
	else {
		printf("PSP in use");
	}
	
}

void check_status_delegate() {
	reportState();
}