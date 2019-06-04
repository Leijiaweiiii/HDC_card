#include "uart_deal.h"

#define UART1_RECVIE_OVERTIME 30
#define UART0_RECVIE_OVERTIME 30

static uint8_t uart0_buff[UART0_BUFF_MAX_LEN];
static uint8_t uart1_buff[UART1_BUFF_MAX_LEN];

uint16_t uart0_buff_length = 0;
uint16_t uart1_buff_length = 0;

uint8_t uart0_wait_time;
uint8_t uart0_wait_end;
uint8_t uart1_wait_time;
uint8_t uart1_wait_end;



GPS_INFO gps_current_info;
GPSResponseDistill gps_recive_info = {0,0,0};
//GPS_RECIVE_STATUS gps_recive_status = RECIVE_GPS_END;
//uint8_t *Gps_uart_data =  gps_recive_info.buffer;
uint16_t u16Revice_Data_length = 0;
uint16_t u16Send_Data_length = 0;


void Uart1TxIntCallback(void)
{

//    Uart_ClrStatus(UARTCH1,UartTC); 
//    u16Send_Data_length++;

//    M0P_UART1->SBUF = gps_recive_info.buffer[u16Send_Data_length];
//    while(FALSE == Uart_GetStatus(UARTCH1,UartTC)){;}
//			
//    if(gps_recive_info.buffer[u16Send_Data_length] == '\0')
//    {

//        memset(gps_recive_info.buffer, 0, sizeof(gps_recive_info.buffer));
//        Gps_Send_flag = 1;  //发送完成
//        GPS_Send_Count = 0;  
//        u16Send_Data_length = 0;
//        gps_recive_succese = 0;
//		  Uart_ClrStatus(UARTCH1,UartTC);   
//        Uart_DisableIrq(UARTCH1,UartTxIrq);
//        Uart_EnableIrq(UARTCH1,UartRxIrq);   
//    }

}


void Uart0TxIntCallback(void)
{
//    u8TxCnt++;
//    if(u8TxCnt_0<=1)
//    {
//        //M0P_UART0->SBUF = u8TxData[1];
//        //M0P_UART1->SBUF = u8TxData[1];
//    }
}

void Uart0ErrIntCallback(void)
{
  
}
void Uart0PErrIntCallBack(void)
{
}
void Uart0CtsIntCallBack(void)
{
}

void Uart1ErrIntCallback(void)
{
  
}
void Uart1PErrIntCallBack(void)
{
}
void Uart1CtsIntCallBack(void)
{
}
/**
 ******************************************************************************
 ** \brief  UART1接收回调函数
 **
 ******************************************************************************/
void Uart1RxIntCallback()
{
    Uart_ClrStatus(UARTCH1, UartRC); 
    uart1_buff[uart1_buff_length] = Uart_ReceiveData(UARTCH1);//数据入队
//    if (uart1_wait_end == 0)
//    {
//        uart1_buff_length = 0;
//        uart1_buff[uart1_buff_length] = Uart_ReceiveData(UARTCH1);//数据入队
//        if (uart1_buff[uart1_buff_length] == '$')//GPS信息开始字符
//        {
//            uart1_wait_time = 30;
//            uart1_wait_end = 1;
//        }
//    }
//    else
//    {
//        if(uart1_wait_time != 0)
//        {
//            uart1_wait_time = 30;
//        }
//        uart1_buff_length ++;
//        
//        uart1_buff[uart1_buff_length] = Uart_ReceiveData(UARTCH1);
//        if(uart1_wait_time == 0)//接收时间到
//        {
//            memset(uart1_buff, 0, strlen((const char*)uart1_buff));
//            uart1_buff_length = 0;
//            uart1_wait_end = 0;
//            Uart_ClrStatus(UARTCH1, UartRC); 
//        }       
//    }
    if (gps_recive_flag == 0)
    {
        gps_recive_info.length = 0;
        gps_recive_info.buffer[gps_recive_info.length] = uart1_buff[uart1_buff_length];//数据出队
        if (gps_recive_info.buffer[gps_recive_info.length] == '$')//GPS信息开始字符
        {
            gps_recive_time = UART1_RECVIE_OVERTIME;
            gps_recive_flag = 1;
        }
    }
    else
    {
        if(gps_recive_time != 0)
        {
            gps_recive_time = UART1_RECVIE_OVERTIME;
        }
        gps_recive_info.length ++;
        
        gps_recive_info.buffer[gps_recive_info.length] = Uart_ReceiveData(UARTCH1);
        if ((gps_recive_info.length == 4)&&(gps_recive_info.buffer[gps_recive_info.length] != 'G'))
        {
            gps_recive_flag = 0;
            gps_recive_time = 0;
        }       
    }
    uart1_buff_length++;
    if(uart1_buff_length >= UART1_BUFF_MAX_LEN)
    {
        uart1_buff_length = 0;
    }
}
/**
 ******************************************************************************
 ** \brief  UART1接收回调函数
 **
 ******************************************************************************/
void Uart0RxIntCallback()
{
    Uart_ClrStatus(UARTCH0, UartRC); 
    uart0_buff[uart0_buff_length] = Uart_ReceiveData(UARTCH0);
    uart0_buff_length++;
    if(uart0_buff_length >= UART0_BUFF_MAX_LEN)
    {
        uart0_buff_length = 0;
    }
    if (at_recive_flag == 0)
    {
        at_response_parser.length = 0;
        at_response_parser.buffer[at_response_parser.length] = uart0_buff[at_response_parser.length];
        at_recive_time = UART0_RECVIE_OVERTIME;
        at_recive_flag = 1;
    }
    else
    {
        if(at_recive_time != 0)
        {
            at_recive_time = UART0_RECVIE_OVERTIME;
        }
        at_response_parser.length ++;
        if(at_response_parser.length == GPRS_MAX_BUFF+2)
        {
            at_recive_succese = 1;
            at_recive_time = 0;
            at_recive_flag = 0;
        }
        
        at_response_parser.buffer[at_response_parser.length] = Uart_ReceiveData(UARTCH0);
        if(at_recive_succese == 1)//接收时间到
        {
            uart0_buff_length = 0;
            Uart_DisableIrq(UARTCH0, UartRxIrq);
            Uart_ClrStatus(UARTCH0, UartRC); 
        }       
    }
}

void Uart1_PortInit(void)
{
    stc_gpio_config_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    stcGpioCfg.enDir = GpioDirOut;//输出
    Gpio_Init(GpioPortD,GpioPin0, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortD,GpioPin0, GpioAf3);//TXD0
    
    stcGpioCfg.enDir = GpioDirIn;//输入
    Gpio_Init(GpioPortD,GpioPin1, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortD,GpioPin1, GpioAf3);//RX
    return ;
}

void Uart0_PortInit(void)
{
    stc_gpio_config_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    stcGpioCfg.enDir = GpioDirOut;//输出
    Gpio_Init(GpioPortA,GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin9, GpioAf1);//TXA9

    stcGpioCfg.enDir = GpioDirIn;//输入
    Gpio_Init(GpioPortA,GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin10, GpioAf1);//RX
    return ;
}

void Uart1Init(void)
{
    uint16_t u16Scnt = 0;
    
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;
    en_uart_mmdorck_t enTb8;

    DDL_ZERO_STRUCT(stcConfig);//结构体初始化
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
	
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);	

    Uart1_PortInit();

    stcUartIrqCb.pfnRxIrqCb   = Uart1RxIntCallback;
    //stcUartIrqCb.pfnTxIrqCb   = Uart1TxIntCallback;
    stcUartIrqCb.pfnRxFEIrqCb = Uart1ErrIntCallback;
    stcUartIrqCb.pfnPEIrqCb   = Uart1PErrIntCallBack;
    stcUartIrqCb.pfnCtsIrqCb  = Uart1CtsIntCallBack;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//模式1
    stcConfig.enStopBit = Uart1bit;  
    stcMulti.enMulti_mode = UartNormal;//正常工作模式
    enTb8 = UartEven;//偶校验
    Uart_SetMMDOrCk(UARTCH1, enTb8);
    stcConfig.pstcMultiMode = &stcMulti;

    Uart_Init(UARTCH1, &stcConfig);
    Uart_SetClkDiv(UARTCH1, Uart8Or16Div);
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode1;
    stcBaud.u32Baud = 9600;
    u16Scnt = Uart_CalScnt(UARTCH1, &stcBaud);
    Uart_SetBaud(UARTCH1, u16Scnt);
    
    Uart_EnableIrq(UARTCH1, UartRxIrq);
    //Uart_DisableIrq(UARTCH0, UartRxIrq);
    Uart_DisableIrq(UARTCH1, UartTxIrq);
    Uart_ClrStatus(UARTCH1, UartRC);
    Uart_EnableFunc(UARTCH1, UartRx);
}

void Uart0Init(void)
{
    uint16_t u16Scnt = 0;
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;
    en_uart_mmdorck_t enTb8;


    DDL_ZERO_STRUCT(stcConfig);//结构体初始化
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);

    Uart0_PortInit();

    stcUartIrqCb.pfnRxIrqCb   = Uart0RxIntCallback;
    //stcUartIrqCb.pfnTxIrqCb   = Uart0TxIntCallback;
    stcUartIrqCb.pfnRxFEIrqCb = Uart0ErrIntCallback;
    stcUartIrqCb.pfnPEIrqCb   = Uart0PErrIntCallBack;
    stcUartIrqCb.pfnCtsIrqCb  = Uart0CtsIntCallBack;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//模式1
    stcConfig.enStopBit = Uart1bit;  
    stcMulti.enMulti_mode = UartNormal;//正常工作模式
    enTb8 = UartEven;//偶校验
    Uart_SetMMDOrCk(UARTCH1, enTb8);
    stcConfig.pstcMultiMode = &stcMulti;

    Uart_Init(UARTCH0, &stcConfig);
    Uart_SetClkDiv(UARTCH0, Uart8Or16Div);
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode1;
    stcBaud.u32Baud = 38400;
    u16Scnt = Uart_CalScnt(UARTCH0, &stcBaud);
    Uart_SetBaud(UARTCH0, u16Scnt);
    Uart_EnableIrq(UARTCH0, UartRxIrq);
    Uart_DisableIrq(UARTCH0, UartTxIrq);
    Uart_ClrStatus(UARTCH0, UartRC);
    Uart_EnableFunc(UARTCH0, UartRx);
}
