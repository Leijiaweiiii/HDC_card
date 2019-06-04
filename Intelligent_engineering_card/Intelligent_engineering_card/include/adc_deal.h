#ifndef __ADC__DEAL__H__
#define __ADC__DEAL__H__
#include "adc.h"
#include "bgr.h"
#include "gpio.h"
#define ADC_DETECTION_INTERVAL(n) (n*60) //secend
#define POWER_CHECK_PORT GpioPortA
#define POWER_CHECK_PIN GpioPin5
extern uint16_t u16AdcRestult0;
extern uint16_t u16AdcRestult2;
extern uint16_t u16AdcRestult5;
extern stc_adc_irq_t stcAdcIrqFlag;

void AdcInit(void);
#endif

