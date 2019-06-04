#include "base_time_deal.h"


void BaseTime0Init(void)
{
    uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_config_t     stcBtBaseCfg;

    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer外设时钟使能
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //定时器模式
    stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv16;                  //PCLK/16
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //自动重载16位计数器/定时器
    stcBtBaseCfg.bEnTog     = TRUE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    
    //stcBtBaseCfg.pfnTim0Cb  = Tim0Int;                    //中断函数入口
    
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化
    
    u16ArrValue = 0xFDD6;
    
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //设置重载值(周期 = 0x10000 - ARR)
    
    u16CntValue = 0xFDD6;
    
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //设置计数初值
    
    Bt_M0_EnTOG_Output(TIM0, FALSE);                         //TIM0 端口输出使能
    
    //Bt_ClearIntFlag(TIM0,BtUevIrq);                       //清中断标志
    
    //EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);               //TIM0中断使能
    
    //Bt_Mode0_EnableIrq(TIM0);                             //使能TIM0中断(模式0时只有一个中断)
    
   
}

