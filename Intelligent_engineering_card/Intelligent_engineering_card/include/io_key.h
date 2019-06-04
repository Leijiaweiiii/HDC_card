#ifndef __IO__KEY__H__
#define __IO__KEY__H__
#include "lpm_deal.h"

#define SOS_KEY_PORT GpioPortB
#define SOS_KEY_PIN GpioPin11

#define HOLD_PRESS_TIME 2000
#define SHORT_PRESS_TIME 15

extern char sos_key;
extern char sos_state;

void SosKeyInit(void);
void SosKeyMonitor(void);

#endif

