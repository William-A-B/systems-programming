#include <stdint.h>
#include "cmsis_armclang.h"

void HardFault_Handler(void);
void HardFault_HandlerC(uint32_t *hardfault_args);

/**
 * HardFaultHandler_C:
 * This is called from the HardFault_Handler (below) with a pointer to the Fault stack
 * as the parameter.  We can then unroll the stack and place them into local variables
 * for ease of reading.
 * The various fault status and address registers help diagnose the cause of the fault.
 * The function ends with a BKPT instruction to pass control back into the debugger.
 * Note: can't be static because it's called from assembly language (the linker
 * resolves the symbol).
 */

void HardFault_HandlerC(uint32_t *hardfault_args) {
	volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
	volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;
	volatile uint32_t _CFSR;
	volatile uint32_t _HFSR;
	volatile uint32_t _DFSR;
	volatile uint32_t _AFSR;
	volatile uint32_t _BFAR;
	volatile uint32_t _MMAR;

	stacked_r0  = ((uint32_t)hardfault_args[0]);
	stacked_r1  = ((uint32_t)hardfault_args[1]);
	stacked_r2  = ((uint32_t)hardfault_args[2]);
	stacked_r3  = ((uint32_t)hardfault_args[3]);
	stacked_r12 = ((uint32_t)hardfault_args[4]);
	stacked_lr  = ((uint32_t)hardfault_args[5]);
	stacked_pc  = ((uint32_t)hardfault_args[6]);
	stacked_psr = ((uint32_t)hardfault_args[7]);
	
	// Suppress "set but not used" warnings
	(void) stacked_r0;
	(void) stacked_r1;
	(void) stacked_r2;
	(void) stacked_r3;
	(void) stacked_r12;
	(void) stacked_lr;
	(void) stacked_pc;
	(void) stacked_psr;
	(void) _CFSR;
	(void) _HFSR;
	(void) _DFSR;
	(void) _AFSR;
	(void) _BFAR;
	(void) _MMAR;

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile uint32_t *)(0xE000ED28)));   
																																									
	// Hard Fault Status Register
	_HFSR = (*((volatile uint32_t *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile uint32_t *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile uint32_t *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile uint32_t *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile uint32_t *)(0xE000ED38)));

	__BKPT(0);
}

/**
 * HardFault_Handler:
 * This function is used as the hard fault handler because its name is the same
 * as the default implementation, and the default is subject to a weak export.
 * The LR return code is tested to see which stack was in use when the hard fault
 * occurred, and the relevant stack pointer is put into r0 before the function
 * above is invoked.
 */

__attribute__((naked))
void HardFault_Handler(void) {
	__asm volatile (
		"TST			lr, #4\n\t"
		"ITE			NE\n\t"
		"MRSNE		r0, PSP\n\t"
		"MRSEQ		r0, MSP\n\t"
		"B				HardFault_HandlerC"
	);
}
