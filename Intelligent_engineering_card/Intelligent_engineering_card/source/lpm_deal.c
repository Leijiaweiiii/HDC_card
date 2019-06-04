#include "lpm_deal.h"

MCU_Status mcu_status = MCU_ON;

void LpmInit(void)
{
    stc_lpm_config_t stcLpmCfg;
    DDL_ZERO_STRUCT(stcLpmCfg);;
    
    stcLpmCfg.enSEVONPEND   = SevPndEnable;//SevPndDisable;
    stcLpmCfg.enSLEEPDEEP = SlpDpEnable;//SlpDpDisable;//
    stcLpmCfg.enSLEEPONEXIT = SlpExtDisable;//SlpExtEnable;//
    
    Lpm_Config(&stcLpmCfg);
}

void VbusCheckInit(void)
{
	 stc_gpio_config_t sosGpioCfg;
    
    /*
    en_gpio_dir_t       enDir;           ///< �˿ڷ�������
    en_gpio_drv_t       enDrv;           ///< �˿�������������
    en_gpio_pupd_t      enPuPd;          ///< �˿�����������
    en_gpio_od_t        enOD;            ///< �˿ڿ�©�������
    en_gpio_ctrl_mode_t enCtrlMode;      ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����
    */
    DDL_ZERO_STRUCT(sosGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    sosGpioCfg.enDir = GpioDirIn;//����
 
    sosGpioCfg.enDrv = GpioDrvL;//����
    
    sosGpioCfg.enPuPd = GpioPd;//����
    
    sosGpioCfg.enOD = GpioOdDisable;//��©�ر�
    
    sosGpioCfg.enCtrlMode = GpioAHB;//AHB����
    
    Gpio_Init(VBUS_CHECK_PORT, VBUS_CHECK_PIN, &sosGpioCfg);
    Gpio_ClearIrq(VBUS_CHECK_PORT, VBUS_CHECK_PIN);
    ///< �򿪲�����Ϊ�½����ж�
    Gpio_EnableIrq(VBUS_CHECK_PORT, VBUS_CHECK_PIN, GpioIrqRising);
    ///< ʹ�ܶ˿�PORTBϵͳ�ж�
    EnableNvic(PORTB_IRQn, IrqLevel3, FALSE);
}

