#include "rtc_deal.h"
#include "nb_iot_module.h"
#include "gps.h"

//uint8_t cyccnt=0;
//uint8_t u8Alarmcnt=0;
//uint8_t flg=0;
//stc_rtc_time_t  stcReadTime;
//char flag = 0;

static void RtcCyclicCallback(void)
{
    //cyccnt++;
    //flg = ~flg;
    //Gpio_WriteOutputIO(GpioPortD,GpioPin5,flg);
    //NbIotPowerOn();
    
}
static void RtcAlarmCallback(void)
{
  //u8Alarmcnt++;
}

void RtcInit(void)
{
    stc_rtc_config_t stcRtcConfig; 
    stc_rtc_irq_cb_t stcIrqCb;
    stc_rtc_time_t  stcTime;
    stc_rtc_alarmset_t stcAlarm;
    stc_rtc_cyc_sel_t   stcCycSel;

    DDL_ZERO_STRUCT(stcRtcConfig);
    DDL_ZERO_STRUCT(stcIrqCb);
    DDL_ZERO_STRUCT(stcAlarm);
    DDL_ZERO_STRUCT(stcTime);
    DDL_ZERO_STRUCT(stcCycSel);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);   //RTC模块时钟打开
    
    Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
    
    stcRtcConfig.enClkSel = RtcClk32768;   //外部32.768k
    stcRtcConfig.enAmpmSel = Rtc24h;       //Rtc12h;//

    stcCycSel.enCyc_sel = RtcPradx;        //由PRDX产生周期中断
    stcCycSel.u8Prdx = 63u;                 //每隔(0.5*u8Prdx)s产生一次周期中断          
    //如果不使用周期中断可以使用低功耗定时器产生中断唤醒低功耗模式，或者采用闹钟方式 周期中断最多只能设置32S
    stcRtcConfig.pstcCycSel = &stcCycSel;
    
#if 0
    Rtc_DisableFunc(RtcCount);
    stcAlarm.u8Minute = 0x59;
    stcAlarm.u8Hour = 0x10;
    stcAlarm.u8Week = 0x02;
    Rtc_DisableFunc(RtcAlarmEn);
    Rtc_EnAlarmIrq(Rtc_AlarmInt_Enable);
    Rtc_SetAlarmTime(&stcAlarm);
    Rtc_EnableFunc(RtcAlarmEn);
#endif
    
    stcTime.u8Year = 0x18;
    stcTime.u8Month = 0x04;
    stcTime.u8Day = 0x16;
    stcTime.u8Hour = 0x10;
    stcTime.u8Minute = 0x58;
    stcTime.u8Second = 0x55;
    stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
    stcRtcConfig.pstcTimeDate = &stcTime;
    
    stcIrqCb.pfnAlarmIrqCb = RtcAlarmCallback;
    stcIrqCb.pfnTimerIrqCb = RtcCyclicCallback;
    stcRtcConfig.pstcIrqCb = &stcIrqCb;
    stcRtcConfig.bTouchNvic = FALSE;
    
    Rtc_DisableFunc(RtcCount);
    Rtc_Init(&stcRtcConfig); 
    
    Rtc_EnableFunc(RtcCount);
}

