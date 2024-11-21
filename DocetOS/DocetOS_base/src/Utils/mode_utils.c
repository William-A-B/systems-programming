#include "/Utils/mode_utils.h"
#include <stdio.h>

void reportState() {
	
	uint32_t PSR = getPSR();
	uint32_t CONTROL = getCONTROL();
	uint32_t PSR_mask = (1 << 9) - 1;
	uint32_t nPRIV_position = 0;
	uint32_t SPSEL_position = 1 << 1;
	
	uint32_t thread_mode = 2;
	uint32_t active_sp_msp = 2;
	uint32_t privileged = 2;
	
	if ((PSR & PSR_mask) == 0) {
		//printf("Thread Mode");
		thread_mode = 1;
		if ((CONTROL >> nPRIV_position) == 0) {
			//printf("Code is privileged");
			privileged = 1;
		}
		else {
			//printf("Code is unprivileged");
			privileged = 0;
		}
	}
	else {
		//printf("Handler Mode");
		thread_mode = 0;
		privileged = 1;
	}
	
	if ((CONTROL >> SPSEL_position) == 0) {
		//printf("Active Stack Pointer = MSP");
		active_sp_msp = 1;
	}
	else {
		//printf("Active Stack Pointer = PSP");
		active_sp_msp = 0;
	}
	
}