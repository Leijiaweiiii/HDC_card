#ifndef __UART__DEAL__
#define __UART__DEAL__

#include "uart.h"
#include "gpio.h"
#include "gprs_message.h"
//#include "mqtt_send_message.h"
#include "gps.h"
#include "timer3_deal.h"
//#ifndef __UART_BUFINFO__
//#define __UART_BUFINFO__
#define UART0_BUFF_MAX_LEN 512
#define UART1_BUFF_MAX_LEN 512
/******************************
** \brief uart������ݶ���
*******************************/
//extern uint8_t uart0_buff[UART0_BUFF_MAX_LEN];
//extern uint8_t uart1_buff[UART1_BUFF_MAX_LEN];

//extern uint16_t uart0_buff_length;
//extern uint16_t uint1_buff_length;

extern uint8_t uart0_wait_time;
extern uint8_t uart0_wait_end;
extern uint8_t uart1_wait_time;
extern uint8_t uart1_wait_end;

extern uint16_t uart0_buff_length;
extern uint16_t uart1_buff_length;

//uart1���ջص�����
void Uart1RxIntCallback(void);
//uart0���ջص�����
void Uart1RxIntCallback(void);
//����1port�ڳ�ʼ��
void Uart1Init(void);

//����0port�ڳ�ʼ��
void Uart0Init(void);
//@} // UartGroup

//#endif


#endif

