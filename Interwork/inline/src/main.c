#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Utils/utils.h"

int main(void) {
	configClock();
	configUSART2(38400);

	uint32_t x=3, y=5, z=0;
	// y = 13x + y
  __asm volatile (
    "ADD r2, %[input], %[input], LSL #3\n\t" /* x*9 */
    "ADD %[out2], r2, %[input], LSL #2\n\t" /* +(x*4) */
		"ADD %[out1], %[out1], r2" /* y = y + 13x */
    : [out1] "+&r" (y), [out2] "=&r" (z)
    : [input] "r" (x)
	  : "r2"
	);

	printf("y = %" PRIu32 "\n", y);

	while(1);
}


//"ADD %[out2], %[input], %[input], LSL #3\n\t" /* x*9 */
//"ADD %[out2], %[out2], %[input], LSL #2\n\t" /* +(x*4) */
//"ADD %[out1], %[out1], %[out2]" /* y = y + 13x */

