#include "buzzer.h"
#include "base_time_deal.h"


void BuzzerInit(void)
{
    stc_gpio_config_t         stcTIM0APort;
//    stc_gpio_config_t         stcTIM0BPort;
    
    DDL_ZERO_STRUCT(stcTIM0APort);
//    DDL_ZERO_STRUCT(stcTIM0BPort);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO 外设时钟使能
    //PA00设置为TIM0_CHA
    //Gpio_ClrAnalogMode(GpioPortA, GpioPin0);
    stcTIM0APort.enDir  = GpioDirOut;
    Gpio_Init(BUZZER_PORT, BUZZER_PIN, &stcTIM0APort);
    Gpio_SetAfMode(BUZZER_PORT,BUZZER_PIN,GpioAf5);
    //PA01设置为TIM0_CHB
    //Gpio_ClrAnalogMode(GpioPortA, GpioPin1);
//    stcTIM0BPort.enDir  = GpioDirOut;
//    Gpio_Init(GpioPortA, GpioPin1, &stcTIM0BPort);
//    Gpio_SetAfMode(GpioPortA,GpioPin1,GpioAf3);
}

void OpenBuzzer(void)
{
    Bt_M0_EnTOG_Output(TIM0, TRUE);  
    Bt_M0_Run(TIM0);                                        
}

void CloseBuzzer(void)
{               
    Bt_M0_EnTOG_Output(TIM0, FALSE);      
    Bt_M0_Stop(TIM0);
}

