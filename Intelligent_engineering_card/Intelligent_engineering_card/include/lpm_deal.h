#ifndef _LPM_DEAL_H_
#define _LPM_DEAL_H_
#include "lpm.h"
#include "gpio.h"

#define VBUS_CHECK_PORT GpioPortB
#define VBUS_CHECK_PIN GpioPin5

/**
 *******************************************************************************
 ** \brief MCU״̬ö������
 ******************************************************************************/
//QȺ��164973950
//TEL��024-85718900
typedef enum 
{
    MCU_ON,
    MCU_LP_OFF,
    MCU_LP_ON,
    MCU_KEY_OFF,    
    MCU_KEY_ON,             
    MCU_RUNING

}MCU_Status;

extern MCU_Status mcu_status;

void LpmInit(void);

void ExitLpm(void);

void VbusCheckInit(void);

void VbusCheck(void);

#endif

