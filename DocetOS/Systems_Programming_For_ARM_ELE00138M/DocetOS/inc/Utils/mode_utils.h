#ifndef MODE_UTILS_H
#define MODE_UTILS_H

#include <stdint.h>
#include "OS/os.h"

#define check_CPU_state(arg0) _svc_1(arg0, SVC_STATUS)

uint32_t getPSR();
uint32_t getCONTROL();
/**
 * @brief Report the state of the CPU
 */
void reportState(void);

#endif /* MODE_UTILS_H */