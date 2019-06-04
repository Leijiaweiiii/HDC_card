#ifndef __RGB__H__
#define __RGB__H__

#define RED_LED_PORT   GpioPortB
#define GREEN_LED_PORT GpioPortB
#define BLUE_LED_PORT  GpioPortA


#define RED_LED_PIN   GpioPin4
#define GREEN_LED_PIN GpioPin3
#define BLUE_LED_PIN  GpioPin11


void RgbInit(void);//初始化
//开红灯
void OpenRedLed(void);
//开绿灯
void OpenGreenLed(void);
//开蓝灯
void OpenBlueLed(void);
//关红灯
void CloseRedLed(void);
//关绿灯
void CloseGreenLed(void);
//关蓝灯
void CloseBlueLed(void);

#endif

