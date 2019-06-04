#ifndef _NB_IOT_MODULE_H_
#define _NB_IOT_MODULE_H_
#include "gpio.h"
#include "ddl.h"

void NbIotInit(void);

void NbIotPowerOn(void);

void NbIotReset(void);

void CloseNbIot(void);
#endif

