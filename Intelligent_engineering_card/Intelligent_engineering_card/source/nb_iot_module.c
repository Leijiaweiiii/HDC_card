#include "nb_iot_module.h"

#define NB_RESET_PORT GpioPortA
#define NB_RESET_PIN  GpioPin4
#define NB_POWER_PORT GpioPortA
#define NB_POWER_PIN  GpioPin2


void NbIotInit(void)
{
    stc_gpio_config_t stcGpioCfg;
    
    /*
    en_gpio_dir_t       enDir;           ///< 端口方向配置
    en_gpio_drv_t       enDrv;           ///< 端口驱动能力配置
    en_gpio_pupd_t      enPuPd;          ///< 端口上下拉配置
    en_gpio_od_t        enOD;            ///< 端口开漏输出配置
    en_gpio_ctrl_mode_t enCtrlMode;      ///< 端口输入/输出值寄存器总线控制模式配置
    */
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    stcGpioCfg.enDir = GpioDirOut;//输出
 
    stcGpioCfg.enDrv = GpioDrvH;//高驱
    
    stcGpioCfg.enPuPd = GpioPd;//下拉
    
    stcGpioCfg.enOD = GpioOdDisable;//开漏关闭
    
    stcGpioCfg.enCtrlMode = GpioAHB;//AHB总线
    
    Gpio_Init(NB_RESET_PORT, NB_RESET_PIN, &stcGpioCfg);
    Gpio_Init(NB_POWER_PORT, NB_POWER_PIN, &stcGpioCfg);
    
    Gpio_ClrIO(NB_POWER_PORT, NB_POWER_PIN);
    Gpio_ClrIO(NB_RESET_PORT, NB_RESET_PIN);
    
}
void NbIotPowerOn(void)
{
    Gpio_SetIO(NB_POWER_PORT, NB_POWER_PIN);
    delay1ms(2500);
    Gpio_ClrIO(NB_POWER_PORT, NB_POWER_PIN);
}

void CloseNbIot(void)
{
    Gpio_SetIO(NB_POWER_PORT, NB_POWER_PIN);
    delay1ms(3000);
    Gpio_ClrIO(NB_POWER_PORT, NB_POWER_PIN);
}

void NbIotReset(void)
{
    Gpio_SetIO(NB_RESET_PORT, NB_RESET_PIN);
    delay1ms(1500);
    Gpio_ClrIO(NB_RESET_PORT, NB_RESET_PIN);
}

