#include "rgb.h"
#include "led.h"
#include "gpio.h"
#include "gprs_message.h"

void RgbInit(void)
{
    stc_gpio_config_t rgbGpioCfg;
    
    DDL_ZERO_STRUCT(rgbGpioCfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    rgbGpioCfg.enDir = GpioDirOut;//Êä³ö
    
    rgbGpioCfg.enDrv = GpioDrvL;//µÍÇý
    
    rgbGpioCfg.enPuPd = GpioPu;//ÉÏÀ­
   
    rgbGpioCfg.enOD = GpioOdDisable;

    rgbGpioCfg.enCtrlMode = GpioAHB;
    
    //Gpio_Init(RED_LED_PORT, RED_LED_PIN, &rgbGpioCfg);
    Gpio_Init(GREEN_LED_PORT, GREEN_LED_PIN, &rgbGpioCfg);
    Gpio_Init(BLUE_LED_PORT, BLUE_LED_PIN, &rgbGpioCfg);
    //Gpio_SetIO(RED_LED_PORT, RED_LED_PIN);
	Gpio_SetIO(GREEN_LED_PORT, GREEN_LED_PIN);
    Gpio_SetIO(BLUE_LED_PORT, BLUE_LED_PIN);
    
}

void OpenRedLed(void)
{
    Gpio_ClrIO(RED_LED_PORT, RED_LED_PIN);
}

void OpenGreenLed(void)
{
    Gpio_ClrIO(GREEN_LED_PORT, GREEN_LED_PIN);
}

void OpenBlueLed(void)
{
    Gpio_ClrIO(BLUE_LED_PORT, BLUE_LED_PIN);
}

void CloseRedLed(void)
{
    Gpio_SetIO(RED_LED_PORT, RED_LED_PIN);
}

void CloseGreenLed(void)
{
    Gpio_SetIO(GREEN_LED_PORT, GREEN_LED_PIN);
}

void CloseBlueLed(void)
{
    Gpio_SetIO(BLUE_LED_PORT, BLUE_LED_PIN);
}

