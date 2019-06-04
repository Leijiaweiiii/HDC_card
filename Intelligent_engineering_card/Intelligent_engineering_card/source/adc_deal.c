#include "adc_deal.h"

uint16_t u16AdcRestult0;
uint16_t u16AdcRestult2;
uint16_t u16AdcRestult5;
stc_adc_irq_t stcAdcIrqFlag;

void AdcContIrqCallback(void)
{    
    Adc_GetJqrResult(&u16AdcRestult5, 2);
	     
    stcAdcIrqFlag.bAdcJQRIrq = TRUE;
 
}


void AdcInit(void)
{
    uint8_t                    u8AdcScanCnt;
    stc_adc_cfg_t              stcAdcCfg;
    stc_adc_irq_t              stcAdcIrq;
    stc_adc_irq_calbakfn_pt_t  stcAdcIrqCalbaks;
    stc_gpio_config_t          stcAdcAN0Port;
    stc_gpio_config_t          stcAdcAN2Port;
    stc_gpio_config_t          stcAdcAN5Port;
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
    DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
    DDL_ZERO_STRUCT(stcAdcIrqFlag);
    DDL_ZERO_STRUCT(stcAdcAN0Port);
    DDL_ZERO_STRUCT(stcAdcAN2Port);
    DDL_ZERO_STRUCT(stcAdcAN5Port);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    
    Gpio_SetAnalogMode(POWER_CHECK_PORT, POWER_CHECK_PIN);        //PA05 (AIN5)
    stcAdcAN5Port.enDir = GpioDirIn;
    Gpio_Init(POWER_CHECK_PORT, POWER_CHECK_PIN, &stcAdcAN5Port);
 
    
    
    if (Ok != Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE))
    {
        return ;
    }    
    
    //ADC
    Adc_Enable();
    //M0P_BGR->CR_f.BGR_EN = 0x1u;                 //BGR
    //M0P_BGR->CR_f.TS_EN = 0x0u;
    Bgr_BgrEnable();
    delay100us(1);
    
    stcAdcCfg.enAdcOpMode = AdcSCanMode;         //
    stcAdcCfg.enAdcClkDiv = AdcClkSysTDiv1;
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime12Clk;
    stcAdcCfg.enAdcRefVolSel   = RefVolSelInBgr2p5;
    stcAdcCfg.bAdcInBufEn      = FALSE;
		
	Adc_SetVref(RefVolSelInBgr2p5);
    Adc_Init(&stcAdcCfg);
    
    Adc_ConfigJqrChannel(JQRCH2MUX, AdcExInputCH5);   
    
    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    Adc_EnableIrq();
    
    stcAdcIrq.bAdcJQRIrq = TRUE;
    stcAdcIrqCalbaks.pfnAdcJQRIrq = AdcContIrqCallback;
    
    Adc_ConfigIrq(&stcAdcIrq, &stcAdcIrqCalbaks);
    
    u8AdcScanCnt = 3;
    
    Adc_ConfigJqrMode(&stcAdcCfg, u8AdcScanCnt, FALSE);
    
    Adc_SQR_Stop();
}

