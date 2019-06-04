#ifndef __RGB__H__
#define __RGB__H__

#define RED_LED_PORT   GpioPortB
#define GREEN_LED_PORT GpioPortB
#define BLUE_LED_PORT  GpioPortA


#define RED_LED_PIN   GpioPin4
#define GREEN_LED_PIN GpioPin3
#define BLUE_LED_PIN  GpioPin11


void RgbInit(void);//��ʼ��
//�����
void OpenRedLed(void);
//���̵�
void OpenGreenLed(void);
//������
void OpenBlueLed(void);
//�غ��
void CloseRedLed(void);
//���̵�
void CloseGreenLed(void);
//������
void CloseBlueLed(void);

#endif

