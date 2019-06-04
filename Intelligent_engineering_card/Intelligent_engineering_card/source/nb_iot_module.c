#include "nb_iot_module.h"

#define NB_RESET_PORT GpioPortA
#define NB_RESET_PIN  GpioPin4
#define NB_POWER_PORT GpioPortA
#define NB_POWER_PIN  GpioPin2


void NbIotInit(void)
{
    stc_gpio_config_t stcGpioCfg;
    
    /*
    en_gpio_dir_t       enDir;           ///< �˿ڷ�������
    en_gpio_drv_t       enDrv;           ///< �˿�������������
    en_gpio_pupd_t      enPuPd;          ///< �˿�����������
    en_gpio_od_t        enOD;            ///< �˿ڿ�©�������
    en_gpio_ctrl_mode_t enCtrlMode;      ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����
    */
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    stcGpioCfg.enDir = GpioDirOut;//���
 
    stcGpioCfg.enDrv = GpioDrvH;//����
    
    stcGpioCfg.enPuPd = GpioPd;//����
    
    stcGpioCfg.enOD = GpioOdDisable;//��©�ر�
    
    stcGpioCfg.enCtrlMode = GpioAHB;//AHB����
    
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

