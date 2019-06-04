#include "gpio.h"
#include "gprs_message.h"
#include "io_key.h"

char sos_key = 0;
char sos_state = FALSE;

void Gpio_IRQHandler(uint8_t u8Param)
{
    ///< PORT B
    printf("Gpio_IRQ B\r\n");
    if(1 == u8Param)
    {
        if(TRUE == Gpio_GetIrqStatus(SOS_KEY_PORT, SOS_KEY_PIN))
        {
            delay1ms(15);
            if(FALSE == Gpio_GetInputIO(SOS_KEY_PORT, SOS_KEY_PIN))
            {
//                Gpio_ClearIrq(SOS_KEY_PORT, SOS_KEY_PIN);
//                EnableNvic(PORTB_IRQn, IrqLevel3, FALSE);
//                if(sos_key == 0)
//                {
//                   sos_key = 1;
//                }
                Gpio_ClearIrq(SOS_KEY_PORT, SOS_KEY_PIN);
                //EnableNvic(PORTB_IRQn, IrqLevel3, FALSE);
            }
        }
    }
}    

void SosKeyInit()
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
    
    sosGpioCfg.enPuPd = GpioPu;//上拉
    
    sosGpioCfg.enOD = GpioOdDisable;//开漏关闭
    
    sosGpioCfg.enCtrlMode = GpioAHB;//AHB总线
    
    Gpio_Init(SOS_KEY_PORT, SOS_KEY_PIN, &sosGpioCfg);
    Gpio_ClearIrq(SOS_KEY_PORT, SOS_KEY_PIN);
    ///< 打开并配置为下降沿中断
    Gpio_EnableIrq(SOS_KEY_PORT, SOS_KEY_PIN, GpioIrqFalling);
    ///< 使能端口PORTD系统中断
    EnableNvic(PORTB_IRQn, IrqLevel0, TRUE);
    //EnableNvic(PORTB_IRQn, IrqLevel3, FALSE);
 
}

void SosKeyMonitor()
{
	 if(TRUE == Gpio_GetInputIO(SOS_KEY_PORT, SOS_KEY_PIN))
     {
         //OpenRedLed();
     }
     else
     {
         delay1ms(15);
         if(FALSE == Gpio_GetInputIO(SOS_KEY_PORT, SOS_KEY_PIN))
         {
             if((mcu_status == MCU_KEY_ON)||(mcu_status == MCU_ON))
             {
                 mcu_status = MCU_KEY_OFF;
                 Gpio_ClearIrq(SOS_KEY_PORT, SOS_KEY_PIN);
                 Gpio_EnableIrq(SOS_KEY_PORT, SOS_KEY_PIN, GpioIrqFalling);
                 EnableNvic(PORTB_IRQn, IrqLevel0, TRUE);
                 CloseNbIot();
                 CloseGps();
                 Lpm_GotoLpmMode();
             }
             //Gpio_ClrIO(GpioPortB, GpioPin4);
             //printf("Are you OK ? ? ? ? ?\r\n");
             if(sos_key == 0)
             {
                sos_key = 1;
             }
         }
     }

}


