#include "timer3_deal.h"

/*******************************************************************************
 * Time3�жϷ�����
 ******************************************************************************/
//QȺ��164973950
//TEL��024-85718900

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
        if(gps_recive_succese == TRUE)//���ճɹ�
        {
            //printf("**********************open*******************\r\n");
            GetGpsValid(&gps_current_info ,gps_recive_info.buffer);
            //OpenGreenLed();
            gps_recive_succese = FALSE;
            if(gps_current_info.valid == TRUE)//�����Ƿ���Ч
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
                gps_sleep.sleepSecend = 20;//������Ч�ػ�5s//������6s��Ϊ����GPS��Ҫ1s
                gps_sleep_ms_count = 0;
                gps_fail_ms_count = 0;
                gps_fail_s_count = 0;
                gps_fail_m_count = 0;
                gps_fail_h_count = 0;
            }
            else if(gps_fail_m_count >= 2)//���������Ӿ�ִ��
            {
                gps_info_flag = 0;
                gps_sleep_h_count = 0;
                gps_sleep_s_count = 0;
                gps_sleep_m_count = 0;
                gps_fail_m_count = 0;
                gps_fail_s_count = 0;
                //printf("*********************close gps*******************\r\n");
                gps_sleep.sleepHour = 0;
                gps_sleep.sleepmin = 1;//�ػ�1����
                gps_sleep.sleepSecend = 0;
                OpenBlueLed();
                Uart_DisableIrq(UARTCH1, UartRxIrq);
                Uart_ClrStatus(UARTCH1, UartRC);
                sos_state = FALSE;//������GPS��Ӧ�����̷���λ�õ��������ղ���GPSֱ�Ӳ���
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
    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //��ʱ��ģʽ
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv64;              //PCLK/64
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //�Զ�����16λ������/��ʱ��
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;
    
    stcTim3BaseCfg.pfnTim3Cb  = Tim3Int;                    //�жϺ������
    
    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 ��ģʽ0���ܳ�ʼ��
        
	u16ArrValue = 0xFD11 - 1;                               //����ֵΪ100us
    
    Tim3_M0_ARRSet(u16ArrValue);                            //��������ֵ(���� = 0x10000-ARR)//���� = 0x6000
    
    u16CntValue = 0xFD11 - 1;                               //��������Ϊ65536 - 10(16λ����ʱ)TH = (65536 - u16CntValue)/256 TL = (65536 - u16CntValue)%256
    
    Tim3_M0_Cnt16Set(u16CntValue);                          //���ü�����ֵ
    
    Tim3_ClearIntFlag(Tim3UevIrq);                          //���жϱ�־
    Tim3_Mode0_EnableIrq();                                 //ʹ��TIM3�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 ���ж� 
    
    Tim3_M0_Run();                                          //TIM3 ���С�
}

