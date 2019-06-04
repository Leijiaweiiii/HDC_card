#include "timer3_deal.h"

/*******************************************************************************
 * Time3中断服务函数
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

uint8_t at_recive_flag = 0;
uint8_t at_recive_succese = 0;
uint8_t at_recive_time = 0;

uint16_t send_gps_waittime = 0;
uint8_t send_gps_flag = 1;
uint8_t gps_recive_time = 0;
uint8_t gps_recive_succese = 0;
uint8_t gps_recive_flag = 0;

uint16_t send_waittime = 0;
uint8_t send_flag = 1;


uint8_t led_b_flag = 0;

uint8_t led_b_time = 0;
uint8_t led_b_count = 0;
uint8_t led_g_flag = 0;
uint8_t led_g_time = 0;
uint8_t led_r_flag = 0;
uint8_t led_r_time = 0;

uint16_t gps_sleep_ms_count = 0;
uint16_t gps_sleep_s_count = 0;
uint16_t gps_sleep_m_count = 0;
uint16_t gps_sleep_h_count = 0;

uint16_t gps_fail_ms_count = 0;
uint16_t gps_fail_s_count = 0;
uint16_t gps_fail_m_count = 0;
uint16_t gps_fail_h_count = 0;
uint8_t gps_info_flag = 1;

uint32_t no_sim_time = 5000;

uint8_t regist_time_ms = 0;
uint8_t regist_time_flag = 0;
uint8_t time_ms = 0;
void Tim3Int(void)
{
    if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
    {
         SosKeyMonitor();
//        if((flash_memory_tidusrpwd_flag == 1)||(flash_memory_id_flag == 1)||(creat_system_id_flag == 1))
//        {
//            time_ms++;
//            if(time_ms == 5)
//            {
//                //printf("regist_time_ms = %d\r\n", regist_time_ms);
//                if(regist_time_flag == 0)
//                {
//                    Pca_Run();
//                    Pca_CmpDataHSet(Module0, regist_time_ms);
//                    regist_time_ms +=5;
//                    if(regist_time_ms >= 255)
//                    {
//                        regist_time_flag = 1;
//                    }
//                }
//                else
//                {
//                    //Pca_Run();
//                    Pca_CmpDataHSet(Module0, regist_time_ms);
//                    regist_time_ms -=5;
//                    if(regist_time_ms <= 0)
//                    {
//                        regist_time_flag = 0;
//                    }
//                }
//                time_ms = 0; 
//            }
//        }
//        else
//        {
//            CloseRedBLN();
//        }
        if(at_response_parser.gsm_state_machine == GSM_NOSIM)
        {
            //printf("*********at_response_parser.gsm_state_machine =%d\r\n", at_response_parser.gsm_state_machine);
            no_sim_time++;
            //printf("*********no_sim_time =%d\r\n", no_sim_time);
            if(no_sim_time >= 5000)
            {
                //printf("buzzer\r\n");
                OpenBuzzer();
                delay1ms(500);
                CloseBuzzer();
                no_sim_time = 0;
            }
        }
        
        if(gps_recive_time)
        {
            gps_recive_time--;
            if(gps_recive_time == 0)
            {
                gps_recive_succese = TRUE;
                gps_recive_flag = FALSE;
                uart1_buff_length = 0;
            }
        }
        if(gps_recive_succese == TRUE)//接收成功
        {
            //printf("**********************open*******************\r\n");
            GetGpsValid(&gps_current_info ,gps_recive_info.buffer);
            //OpenGreenLed();
            gps_recive_succese = FALSE;
            if(gps_current_info.valid == TRUE)//数据是否有效
            {
               
                gps_current_info.valid = FALSE;
               // CloseGreenLed();
                GPS_Analysis(&gps_current_info ,gps_recive_info.buffer);
                //printf("**********************%f*******************", gps_current_info.height);
                //printf("*********************close gps*******************\r\n");
                EnQueue(gps_current_info, &gps_queue);
                Uart_DisableIrq(UARTCH1, UartRxIrq);
                Uart_ClrStatus(UARTCH1, UartRC);
                CloseGps();
                gps_sleep.sleepHour = 0;
                gps_sleep.sleepmin = 0;
                gps_sleep.sleepSecend = 20;//数据有效关机5s//会运行6s因为接收GPS需要1s
                gps_sleep_ms_count = 0;
                gps_fail_ms_count = 0;
                gps_fail_s_count = 0;
                gps_fail_m_count = 0;
                gps_fail_h_count = 0;
            }
            else if(gps_fail_m_count >= 2)//超过两分钟就执行
            {
                gps_info_flag = 0;
                gps_sleep_h_count = 0;
                gps_sleep_s_count = 0;
                gps_sleep_m_count = 0;
                gps_fail_m_count = 0;
                gps_fail_s_count = 0;
                //printf("*********************close gps*******************\r\n");
                gps_sleep.sleepHour = 0;
                gps_sleep.sleepmin = 1;//关机1分钟
                gps_sleep.sleepSecend = 0;
                OpenBlueLed();
                Uart_DisableIrq(UARTCH1, UartRxIrq);
                Uart_ClrStatus(UARTCH1, UartRC);
                sos_state = FALSE;//发送完GPS后应该立刻发送位置但是由于收不到GPS直接不发
                CloseGps();
            }
            memset(gps_recive_info.buffer, 0, strlen((const char*)gps_recive_info.buffer));
        }
        if((gps_sleep.sleepSecend > 0)||(gps_sleep.sleepHour > 0)||(gps_sleep.sleepmin > 0))
        {
            if((gps_sleep.sleepSecend == gps_sleep_s_count)&&
                (gps_sleep.sleepmin == gps_sleep_m_count)&&
                (gps_sleep.sleepHour == gps_sleep_h_count))
            {
                gps_info_flag = 1;
                gps_sleep_h_count = 0;
                gps_sleep_s_count = 0;
                gps_sleep_m_count = 0;
                gps_sleep.sleepSecend = 0;
                gps_sleep.sleepmin = 0;
                gps_sleep.sleepHour = 0;
                //printf("********************open gps*******************\r\n");
                OpenGps();
                Uart_EnableIrq(UARTCH1,UartRxIrq);
            }
            gps_sleep_ms_count ++;
            if(gps_sleep_ms_count == 999)
            {
                gps_sleep_s_count++;
                gps_sleep_ms_count = 0;
                if(gps_sleep_s_count == 60)
                {
                    gps_sleep_m_count++;
                    gps_sleep_s_count = 0;
                }
                else if(gps_sleep_m_count == 60)
                {
                    gps_sleep_h_count ++;
                    gps_sleep_m_count = 0;
                }
            }
        }
        if((gps_current_info.valid == FALSE) && gps_info_flag == 1)
        {
            gps_fail_ms_count ++;
            if(gps_fail_ms_count == 1000)
            {
                gps_fail_s_count++;
                gps_fail_ms_count = 0;
                if(gps_fail_s_count == 60)
                {
                    gps_fail_m_count++;
                    gps_fail_s_count = 0;
                    if(gps_fail_m_count == 60)
                    {
                        gps_fail_h_count++;
                        gps_fail_m_count = 0;
                    }
                }
            }
        }
        if(at_recive_time)
        {
            at_recive_time--;
            if(at_recive_time == 0)
            {
                at_recive_succese = TRUE;
                uart0_buff_length = FALSE;
                at_recive_flag = FALSE;
                Uart_DisableIrq(UARTCH0, UartRxIrq);
                Uart_ClrStatus(UARTCH0, UartRC); 
            }
        }
        if(send_waittime)
        {
            send_waittime--;
            if(send_waittime == 0)
            {
                send_flag = TRUE;
            }
        }
        if(gprs_send_parser.overtime)
        {
            gprs_send_parser.overtime--;
            if(gprs_send_parser.overtime == 0)
            {
                gprs_send_parser.overtime_flag = TRUE;
            }
        }
        if(send_gps_waittime)
        {
            send_gps_waittime--;
            if(send_gps_waittime == 0)
            {
                send_gps_flag = 1;
            }
        }
        Tim3_ClearIntFlag(Tim3UevIrq);
    }
}

void Time3_Init(void)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_config_t     stcTim3BaseCfg;

    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //定时器模式
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //定时器功能，计数时钟为内部PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv64;              //PCLK/64
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //自动重载16位计数器/定时器
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;
    
    stcTim3BaseCfg.pfnTim3Cb  = Tim3Int;                    //中断函数入口
    
    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 的模式0功能初始化
        
	u16ArrValue = 0xFD11 - 1;                               //重载值为100us
    
    Tim3_M0_ARRSet(u16ArrValue);                            //设置重载值(周期 = 0x10000-ARR)//周期 = 0x6000
    
    u16CntValue = 0xFD11 - 1;                               //计数次数为65536 - 10(16位计数时)TH = (65536 - u16CntValue)/256 TL = (65536 - u16CntValue)%256
    
    Tim3_M0_Cnt16Set(u16CntValue);                          //设置计数初值
    
    Tim3_ClearIntFlag(Tim3UevIrq);                          //清中断标志
    Tim3_Mode0_EnableIrq();                                 //使能TIM3中断(模式0时只有一个中断)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 开中断 
    
    Tim3_M0_Run();                                          //TIM3 运行。
}

