#ifndef __BUZZER__H__
#define __BUZZER__H__

#include "timer3_deal.h"

#define BUZZER_PORT GpioPortA
#define BUZZER_PIN GpioPin15

void BuzzerInit(void);//��ʼ��
void OpenBuzzer(void);//��������
void CloseBuzzer(void);//�ط�����
#endif

