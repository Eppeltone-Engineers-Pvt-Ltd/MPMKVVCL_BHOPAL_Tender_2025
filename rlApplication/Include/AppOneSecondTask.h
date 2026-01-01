#ifndef __APP_ONESECONDTASK_H
#define __APP_ONESECONDTASK_H

#include "..\\..\\rlDevice\\Include\\dTypedef.h"
#include "AppVariables.h"
void OneSecondTask(void);
void getMD_LSPulseCounter(void);
void getMD_LSPulseCounter_SD(void);
uint16_t checkkWh2kVAh(uint16_t blkkwh,uint16_t blkkVAh);
uint8_t isIntevalCrossed(uint32_t interval);

#endif
