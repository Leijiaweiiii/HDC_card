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
    en_gpio_dir_t       enDir;           ///< 端口方向配置
    en_gpio_drv_t       enDrv;           ///< 端口驱动能力配置
    en_gpio_pupd_t      enPuPd;          ///< 端口上下拉配置
    en_gpio_od_t        enOD;            ///< 端口开漏输出配置
    en_gpio_ctrl_mode_t enCtrlMode;      ///< 端口输入/输出值寄存器总线控制模式配置
    */
    DDL_ZERO_STRUCT(sosGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    sosGpioCfg.enDir = GpioDirIn;//输入
 
    sosGpioCfg.enDrv = GpioDrvL;//低驱
    
    sosGpioCfg.enPuPd = GpioPd;//上拉
    
    sosGpioCfg.enOD = GpioOdDisable;//开漏关闭
    
    sosGpioCfg.enCtrlMode = GpioAHB;//AHB总线
    
    Gpio_Init(VBUS_CHECK_PORT, VBUS_CHECK_PIN, &sosGpioCfg);
    Gpio_ClearIrq(VBUS_CHECK_PORT, VBUS_CHECK_PIN);
    ///< 打开并配置为下降沿中断
    Gpio_EnableIrq(VBUS_CHECK_PORT, VBUS_CHECK_PIN, GpioIrqRising);
    ///< 使能端口PORTB系统中断
    EnableNvic(PORTB_IRQn, IrqLevel3, FALSE);
}

