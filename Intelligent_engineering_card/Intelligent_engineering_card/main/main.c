/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
//Q群：164973950
//TEL：024-85718900
/******************************************************************************/
//Q群：164973950
//TEL：024-85718900
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2017-05-17  1.0  cj First version for Device Driver Library of Module.
 **
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Include files
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900
#include "ddl.h"
#include "uart_deal.h"
#include "rgb.h"
#include "sysctrl.h"
#include "stdio.h"
#include "timer3_deal.h"
#include "lpm_deal.h"
#include "flash.h"
#include "io_key.h"
#include "nb_iot_module.h"
#include "gprs_message.h"
#include "base_time_deal.h"
#include "rtc_deal.h"
#include "adc_deal.h"
#include "pca_deal.h"
#include "buzzer.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                            
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Local type definitions ('typedef')                                         
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')                             
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900
#define     T1_PORT                 (3)
#define     T1_PIN                  (3)
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900
/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
//Q群：164973950
//TEL：024-85718900
double sum, sum_OK;
int32_t main(void)
{  
    Sys_Clk();
	Uart0Init();
    Uart1Init();
    RtcInit();	
    NbIotInit();
    NbIotPowerOn();
    GpsInit();
    OpenGps();
    LpmInit();
    AdcInit();
    PcaInit();
//    
    Time3_Init();
    BaseTime0Init();
    BuzzerInit();
    SosKeyInit();
    RgbInit();  
/*****************test Buzzer**************/ 
//    OpenBuzzer();
//    delay1ms(500);
//    CloseBuzzer();
/*****************test Buzzer**************/  
//    while(1)
//    {
//        while(stcAdcIrqFlag.bAdcJQRIrq == FALSE);
//        stcAdcIrqFlag.bAdcJQRIrq = FALSE;
//        sum=(u16AdcRestult5*1000)/(4095+1);
//        sum = (sum*2.5)/1000;
//        sum_OK = ((75*100)/126);
//        sum = (sum*100)/sum_OK;
//        printf("%dV\r\n",u16AdcRestult5);
//        Adc_JQR_Start();
//    }
    Flash_Init(FlashInt, 12, TRUE);
    ReadFlashData();
    while(AtInit(SM_AT_SENDING) == Error);
    GprsHttpSendInfo();
    MqttSendInfo();
//    while(1)
//    {
//        /***************************固定波特率版本*****************************************/
//        while(1)
//        {
//            if(at_recive_succese)
//            {
//                if (strstr((const char *)at_response_parser.buffer, (const char *)"+PBREADY") != NULL)
//                    break;
//            }
//        }
//        if(gps_recive_succese == 1)
//        {
//            while(AtInit() == Error);
//            while(1)
//            { 
//                GprsHttpSendInfo();   
//                if((gprs_send_data_state == GPRS_HTTPS_SENDGPSINFO_OK)||(gprs_send_data_state == GPRS_HTTPS_SENDGPSINFO_ACTION))
//                {
//                    gps_recive_info.length = 0;
//                    gps_recive_succese = 0;
//                    gps_recive_flag = 0;
//                    gps_info_send_statu = 0;
//                    //led_b_count = 0;
//                    memset(gps_recive_info.buffer, 0, sizeof(gps_recive_info.buffer));
//                    Uart_EnableIrq(UARTCH1,UartRxIrq);
//                }
//            }
//        }
//    }	
}


