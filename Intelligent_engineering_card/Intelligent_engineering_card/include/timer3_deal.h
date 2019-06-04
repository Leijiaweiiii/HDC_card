#ifndef __TIME3__DEAL__H__
#define __TIME3__DEAL__H__

#include "timer3.h"
#include "gps.h"
#include "gpio.h"
#include "io_key.h"
#include "uart_deal.h"
#include "arry_queue.h"
#include "buzzer.h"
#include "pca_deal.h"

extern uint8_t gps_recive_time;    //接收时间
extern uint8_t gps_recive_flag;    
extern uint8_t gps_recive_succese;
extern uint8_t GPS_Recive_Count;//接收数据计数
extern uint8_t at_recive_succese;
extern uint8_t at_recive_flag;
extern uint8_t at_recive_time;
extern uint16_t send_waittime;
extern uint8_t led_b_time;
extern uint8_t send_flag;
extern uint16_t send_gps_waittime;
extern uint8_t send_gps_flag;
extern uint32_t no_sim_time;
void Time3_Init(void);

#endif

