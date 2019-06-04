#ifndef __BUZZER__H__
#define __BUZZER__H__

#include "timer3_deal.h"

#define BUZZER_PORT GpioPortA
#define BUZZER_PIN GpioPin15

void BuzzerInit(void);//初始化
void OpenBuzzer(void);//开蜂鸣器
void CloseBuzzer(void);//关蜂鸣器
#endif

