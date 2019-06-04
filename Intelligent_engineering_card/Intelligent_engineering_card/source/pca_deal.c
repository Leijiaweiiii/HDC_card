#include "pca_deal.h"
/******************************������**************************/
static volatile uint32_t u32PcaTestFlag = 0;
static volatile uint16_t u16CcapData[8] = {0};
static uint8_t  u8CmpHData = 0x90;
static uint8_t  u8CmpLData = 0x20;
void PcaInt(void)
{
    if (TRUE == Pca_GetCntIntFlag())
    {
        Pca_ClearCntIntFlag();
        u32PcaTestFlag |= 0x20;
    }
    if (TRUE == Pca_GetIntFlag(Module0))
    {
        Pca_ClearIntFlag(Module0);
        u32PcaTestFlag |= 0x01;
    }
    if (TRUE == Pca_GetIntFlag(Module1))
    {
        Pca_ClearIntFlag(Module1);
        u32PcaTestFlag |= 0x02;
    }
    if (TRUE == Pca_GetIntFlag(Module2))
    {
        Pca_ClearIntFlag(Module2);
        u32PcaTestFlag |= 0x04;
    }
    if (TRUE == Pca_GetIntFlag(Module3))
    {
        Pca_ClearIntFlag(Module3);
        u32PcaTestFlag |= 0x08;
    }
    if (TRUE == Pca_GetIntFlag(Module4))
    {
        Pca_ClearIntFlag(Module4);
        u32PcaTestFlag |= 0x10;
    }
        
}


en_result_t PcaInit(void)
{
    stc_pca_config_t        stcConfig;
    stc_pca_capmodconfig_t  stcModConfig;
    stc_gpio_config_t       stcPCA0Port;
    u8CmpLData = 0;
    u8CmpHData = 0;
//    u8CmpLData = 0x20;
//    u8CmpHData = 0x90;
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcModConfig);
    DDL_ZERO_STRUCT(stcPCA0Port);
    
    //PCA��GPIO����ʱ�ӿ���
    Sysctrl_SetPeripheralGate(SysctrlPeripheralPca, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    //INT ENABLE
    //EnableNvic(PCA_IRQn, 3, TRUE);
    //Pca_EnableIrq(Module0);
//    EnableNvic(PCA_IRQn, 3, FALSE);
//    Pca_DisableIrq(Module0);

    //GPIO CFG
    //PA06����ΪPCA_CH0
    //Gpio_ClrAnalogMode(GpioPortA, GpioPin6);
    stcPCA0Port.enDrv  = GpioDrvH;
    stcPCA0Port.enDir  = GpioDirOut;
    Gpio_Init(GpioPortB, GpioPin4, &stcPCA0Port);
    Gpio_SetAfMode(GpioPortB, GpioPin4, GpioAf2);
    
    stcConfig.enCIDL = IdleGoOn;                   //����ģʽPCA���� 
    stcConfig.enWDTE = PCAWDTDisable;              //wdt���ܹر�
    stcConfig.enCPS  = PCAPCLKDiv32;               //PCLK/32
    
    //stcConfig.pfnPcaCb = PcaInt;                   //�жϺ������
    
    stcModConfig.enECOM     = ECOMEnable;          //�Ƚ�����������
    stcModConfig.enCAPP     = CAPPDisable;         //�����ز����ֹ
    stcModConfig.enCAPN     = CAPNDisable;         //�½��ز����ֹ
    stcModConfig.enMAT      = MATDisable;          //��ֹƥ��
    stcModConfig.enTOG      = TOGDisable;          //��ֹ��ת
    stcModConfig.en8bitPWM  = PCA8bitPWMEnable;    //����8λPWM����
    
    
    if (Ok != Pca_Init(&stcConfig))
    {
        return Error;
    }
    if (Ok != Pca_CapModConfig(Module0, &stcModConfig))
    {
        return Error;
    }
    
    Pca_CmpDataLSet(Module0, u8CmpLData);        //�ȽϼĴ�����ʼ��
    Pca_CmpDataHSet(Module0, u8CmpHData);        //(�Ƚ�����ֵ)�ȽϼĴ�������
    Gpio_SetIO(GpioPortB, GpioPin4);
    Pca_Stop();
    return Ok;
}


void CloseRedBLN(void)
{
    u8CmpHData = 0;
    Pca_CmpDataHSet(Module0, u8CmpHData);
    //Sysctrl_SetPeripheralGate(SysctrlPeripheralPca, FALSE);
    delay1ms(5);
    Pca_Stop();
}

void OpenRedBLN(void)
{
    u8CmpLData = 0x20;
    u8CmpHData = 0x90;
    //Sysctrl_SetPeripheralGate(SysctrlPeripheralPca, TRUE);
    Pca_CmpDataLSet(Module0, u8CmpLData);
    Pca_CmpDataHSet(Module0, u8CmpHData);
    Pca_Run();
}

