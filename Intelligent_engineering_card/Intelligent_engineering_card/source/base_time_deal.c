#include "base_time_deal.h"


void BaseTime0Init(void)
{
    uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_config_t     stcBtBaseCfg;

    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer����ʱ��ʹ��
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //��ʱ��ģʽ
    stcBtBaseCfg.enCT       = BtTimer;                      //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv16;                  //PCLK/16
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //�Զ�����16λ������/��ʱ��
    stcBtBaseCfg.bEnTog     = TRUE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    
    //stcBtBaseCfg.pfnTim0Cb  = Tim0Int;                    //�жϺ������
    
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 ��ģʽ0���ܳ�ʼ��
    
    u16ArrValue = 0xFDD6;
    
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //��������ֵ(���� = 0x10000 - ARR)
    
    u16CntValue = 0xFDD6;
    
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //���ü�����ֵ
    
    Bt_M0_EnTOG_Output(TIM0, FALSE);                         //TIM0 �˿����ʹ��
    
    //Bt_ClearIntFlag(TIM0,BtUevIrq);                       //���жϱ�־
    
    //EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);               //TIM0�ж�ʹ��
    
    //Bt_Mode0_EnableIrq(TIM0);                             //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    
   
}

