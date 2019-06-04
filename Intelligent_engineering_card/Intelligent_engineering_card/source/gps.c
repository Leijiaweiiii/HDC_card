#include "gps.h"
#include "gpio.h"
#include "uart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "math.h"
#include "hc32l136.h"
#include "timer3_deal.h"
#include "rgb.h"
//////////////////////////////////////////////////////////////////////////////////

#define GPS_POWER_PORT GpioPortA
#define GPS_POWER_PIN  GpioPin6
DevNV_GPS_Sleep gps_sleep;

void GpsInit(void)
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
    
    Gpio_Init(GPS_POWER_PORT, GPS_POWER_PIN, &stcGpioCfg);
    
    Gpio_ClrIO(GPS_POWER_PORT, GPS_POWER_PIN);
    
}
void OpenGps(void)
{
    Gpio_SetIO(GPS_POWER_PORT, GPS_POWER_PIN);
}
void CloseGps(void)
{
    Gpio_ClrIO(GPS_POWER_PORT, GPS_POWER_PIN);
}


uint8_t Nmea_Comma_Pos(uint8_t *buf,uint8_t cx)
{ 
    uint8_t *p=buf;
    while(cx)
    { 
        if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
        //遇到'*'或者非法字符,则不存在第 cx 个逗号
        if(*buf==',')cx--;
        buf++;
    }
    return buf-p;
}
//m^n 函数
//返回值:m^n 次方.
uint32_t Nmea_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
	
    while(n--) result*=m;
    return result;
}
//str 转换为数字,以','或者'*'结束
//buf:数字存储区
//dx:小数点位数,返回给调用函数
//返回值:转换后的数值
int Nmea_Str2num(uint8_t *buf,uint8_t*dx)
{
    uint8_t *p=buf;
    uint32_t ires=0,fres=0;
    uint8_t ilen=0,flen=0,i;
    uint8_t mask=0;
    int res;
	
    while(1) //得到整数和小数的长度
    {
        if (*p=='-') {mask|=0X02;p++;}      //是负数
        if((*p==',') || (*p=='*')) break;   //遇到结束了
        if (*p=='.') {mask|=0X01;p++;}      //遇到小数点了
        else if(*p>'9' || (*p<'0'))           //有非法字符
        { 
            ilen=0;
            flen=0;
            break;
        } 
        if(mask&0X01)flen++;
        else ilen++;
        p++;
    }
		//printf("ilen = %u\n", ilen);
    if(mask&0X02) buf++; //去掉负号
    for(i=0; i<ilen; i++) //得到整数部分数据
    {
        ires+=Nmea_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen>5)flen=5;  //最多取 5 位小数
    *dx = flen; //小数点位数
    for(i=0; i<flen; i++) //得到小数部分数据
    {
        fres += Nmea_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
    }
    res = ires*Nmea_Pow(10,flen)+fres;
    if(mask&0X02) res=-res;
		//printf("res = %u\n", res);
    return res;
} 

//分析 GPGSV 信息
//gpsx:nmea 信息结构体
//buf:接收到的 GPS 数据缓冲区首地址
void Nmea_GPGSV_Analysis(GPS_INFO *gpsx,uint8_t *buf)
{
//    uint8_t *p,*p1,dx;
//    uint8_t len,i,j,slx=0;
//    uint8_t posx;
//    p=buf;
//    p1=(uint8_t*)strstr((const char *)p,"$GPGSV");
//    len=p1[7]-'0'; //得到 GPGSV 的条数
//    posx=Nmea_Comma_Pos(p1,3); //得到可见卫星总数
//    if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
//    for(i=0;i<len;i++)
//    { 
//        p1=(u8*)strstr((const char *)p,"$GPGSV");
//        for(j=0;j<4;j++)
//        { 
//            posx=NMEA_Comma_Pos(p1,4+j*4);
//            if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星编号
//            else break;
//            posx=NMEA_Comma_Pos(p1,5+j*4);
//            if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星仰角
//            else break;
//            posx=NMEA_Comma_Pos(p1,6+j*4);
//            if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星方位角
//            else break;
//            posx=NMEA_Comma_Pos(p1,7+j*4);
//            if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx); 
//            //得到卫星信噪比
//            else break;
//            slx++; 
//       }
//       p=p1+1;//切换到下一个 GPGSV 信息
//    }
//    
}
//分析 BDGSV 信息
//gpsx:nmea 信息结构体
//buf:接收到的北斗数据缓冲区首地址
void Nmea_BDGSV_Analysis(GPS_INFO *gpsx,uint8_t *buf)
{
//    uint8_t *p,*p1,dx;
//    uint8_t len,i,j,slx=0;
//    uint8_t posx;
//    p=buf;
//    p1=(uint8_t*)strstr((const char *)p,"$BDGSV");
//    len=p1[7]-'0'; //得到 BDGSV 的条数
//    posx=Nmea_Comma_Pos(p1,3); //得到可见北斗卫星总数
//    if(posx!=0XFF)gpsx->beidou_svnum=NMEA_Str2num(p1+posx,&dx);
//    for(i=0;i<len;i++)
//    { 
//        p1=(u8*)strstr((const char *)p,"$BDGSV");
//        for(j=0;j<4;j++)
//        { 
//            posx=Nmea_Comma_Pos(p1,4+j*4);
//            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_num=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星编号
//            else break;
//            posx=Nmea_Comma_Pos(p1,5+j*4);
//            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_eledeg=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星仰角
//            else break;
//            posx=Nmea_Comma_Pos(p1,6+j*4);
//            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_azideg=NMEA_Str2num(p1+posx,&dx);
//            //得到卫星方位角
//            else break;
//            posx=Nmea_Comma_Pos(p1,7+j*4);
//            if(posx!=0XFF)gpsx->beidou_slmsg[slx].beidou_sn=NMEA_Str2num(p1+posx,&dx); 
//            //得到卫星信噪比
//            else break;
//            slx++; 
//        }
//        p=p1+1;//切换到下一个 BDGSV 信息
//    }
}
//分析 GNGGA 信息
//gpsx:nmea 信息结构体
//buf:接收到的 GPS/北斗数据缓冲区首地址
void Nmea_GNGGA_Analysis(GPS_INFO *gpsx, uint8_t *buf)
{
    uint8_t *p1,dx; 
    uint8_t posx;
    p1=(uint8_t*)strstr((const char *)buf,"$GPGGA");
    //posx=Nmea_Comma_Pos(p1,6); //得到 GPS 状态
    //if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);
    //posx=Nmea_Comma_Pos(p1,7);
    //得到用于定位的卫星数
    //if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx);
    posx=Nmea_Comma_Pos(p1,8);
	if(posx!=0XFF)
    {
        gpsx->accuracy = Nmea_Str2num(p1+posx,&dx);
        gpsx->accuracy /= 100.0; 
    }
    posx=Nmea_Comma_Pos(p1,9); //得到海拔高度
    if(posx!=0XFF)
    {
        gpsx->height = Nmea_Str2num(p1+posx,&dx);
        gpsx->height /= 10.0;
    }

}
//分析 GNGSA 信息
//gpsx:nmea 信息结构体
//buf:接收到的 GPS/北斗数据缓冲区首地址
void Nmea_GNGSA_Analysis(GPS_INFO *gpsx, uint8_t *buf)
{
//    uint8_t *p1,dx; 
//    uint8_t posx;
//    uint8_t i;
//    p1=(uint8_t*)strstr((const char *)buf, "$GPGSA");
//    posx=Nmea_Comma_Pos(p1,2); //得到定位类型
//    if(posx!=0XFF)gpsx->fixmode=Nmea_Str2num(p1+posx,&dx); 
//    for(i=0;i<12;i++) //得到定位卫星编号
//    {
//        posx=Nmea_Comma_Pos(p1,3+i); 
//        if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
//        else break;
//    }
//    
//    posx=Nmea_Comma_Pos(p1,15); 
//    //得到 PDOP 位置精度因子
//    if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);
//    posx=Nmea_Comma_Pos(p1,16); 
//    //得到 HDOP 位置精度因子
//    if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);
//    posx=Nmea_Comma_Pos(p1,17); 
//    //得到 VDOP 位置精度因子
//    if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);
}
//分析 GNRMC 信息
//gpsx:nmea 信息结构体
//buf:接收到的 GPS/北斗数据缓冲区首地址
void Nmea_GNRMC_Analysis(GPS_INFO *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx; 
    uint8_t posx;
    uint32_t temp; 
    float rs;
    p1=(uint8_t*)strstr((const char *)buf, "$GPRMC");
    //"$GNRMC",经常有&和 GNRMC 分开的情况,故只判断 GPRMC.
	  //printf("%s", p1);
    posx=Nmea_Comma_Pos(p1,1);
    //得到 UTC 时间
	  //printf("posx = %u\n", posx);
    if(posx!=0XFF)
    {
			  
        temp = Nmea_Str2num(p1+posx,&dx);
        temp /= Nmea_Pow(10,dx);			
		//printf("dx = %u", dx);
        //得到 UTC 时间,去掉 ms
		//printf("temp = %u\n", temp);
        gpsx->utc_time.hour=temp/10000;
        gpsx->utc_time.minute=(temp/100)%100;
        gpsx->utc_time.second=temp%100; 
		//printf("utc_time.second = %d\n", gpsx->utc_time.second);
    } 
    posx=Nmea_Comma_Pos(p1,2); 
    if(posx!=0XFF)gpsx->valid=*(p1+posx);
    if(gpsx->valid == 'V')//判断是否有效
    {
        gpsx->valid = 0;
    }
    else if(gpsx->valid == 'A')
    {
        gpsx->valid = 1;
    }
    posx=Nmea_Comma_Pos(p1,3);
    //得到纬度
    if(posx!=0XFF)
    {
        temp = Nmea_Str2num(p1+posx,&dx);
        gpsx->latitude = temp/Nmea_Pow(10,dx+2);  //得到°
        rs = temp%Nmea_Pow(10,dx+2); //得到' 
        gpsx->latitude = (gpsx->latitude*Nmea_Pow(10,5)+(rs*Nmea_Pow(10,5-dx))/60)/100000.0;
        //转换为°
    }
    posx = Nmea_Comma_Pos(p1,4); //南纬还是北纬
    if(posx!=0XFF) gpsx->NS = *(p1+posx);
    if(gpsx->NS == 'S')//南纬为负数
    {
        gpsx->latitude = 0 - gpsx->latitude;
    }
    posx = Nmea_Comma_Pos(p1,5); //得到经度
    if(posx!=0XFF)
    { 
        temp = Nmea_Str2num(p1+posx,&dx);
        gpsx->longitude = temp/Nmea_Pow(10,dx+2);  //得到°
        rs = temp%Nmea_Pow(10,dx+2); //得到' 
        gpsx->longitude = (gpsx->longitude*Nmea_Pow(10,5)+(rs*Nmea_Pow(10,5-dx))/60)/100000;
        //转换为°
    }
    posx=Nmea_Comma_Pos(p1,6); //东经还是西经
    if(posx!=0XFF)gpsx->EW=*(p1+posx);
    if(gpsx->EW == 'W')//西经为负
    {
        gpsx->longitude = 0 - gpsx->longitude;
    }
    posx = Nmea_Comma_Pos(p1,8);
    if(posx!=0XFF)
    {
        gpsx->heading = Nmea_Str2num(p1+posx,&dx);
        gpsx->heading /= 100.0;
        
    }
    posx=Nmea_Comma_Pos(p1,9); //得到 UTC 日期
    if(posx!=0XFF)
    {
        temp = Nmea_Str2num(p1+posx,&dx); //得到 UTC 日期
        gpsx->utc_time.day = temp/10000;
        gpsx->utc_time.month = (temp/100)%100;
        gpsx->utc_time.year = 2000+temp%100;
    }

    if(posx!=0XFF)
    {
        temp = Nmea_Str2num(p1+posx,&dx); //得到 UTC 日期
        gpsx->utc_time.day = temp/10000;
        gpsx->utc_time.month = (temp/100)%100;
        gpsx->utc_time.year = 2000+temp%100;
    }
}
//分析 GNVTG 信息
//gpsx:nmea 信息结构体
//buf:接收到的 GPS/北斗数据缓冲区首地址
void Nmea_GNVTG_Analysis(GPS_INFO *gpsx,uint8_t *buf)
{
    uint8_t *p1,dx; 
    uint8_t posx;
    p1=(uint8_t*)strstr((const char *)buf,"$GPVTG"); 
    posx=Nmea_Comma_Pos(p1,7); //得到地面速率
    if(posx!=0XFF)
    {
        gpsx->speed=Nmea_Str2num(p1+posx,&dx);
        if(dx<3)gpsx->speed*=Nmea_Pow(10,3-dx); 
        gpsx->speed /= 1000.00;
        //确保扩大 1000 倍
    }
}

void GetGpsValid(GPS_INFO *gpsx,uint8_t *buf)
{
    uint8_t *p1; 
    uint8_t posx;
    p1=(uint8_t*)strstr((const char *)buf, "$GPRMC");
    posx=Nmea_Comma_Pos(p1,2); 
    if(posx!=0XFF)gpsx->valid=*(p1+posx);

    if(gpsx->valid == 'V')//判断是否有效
    {
        gpsx->valid = FALSE;
        
    }
    else if(gpsx->valid == 'A')
    {
        gpsx->valid = TRUE;
    }
}

void CheckGpsInfoValue(void)
{
    Uart_EnableIrq(UARTCH1,UartRxIrq);
    while(1)
    {
        if(gps_recive_succese == TRUE)
        {
            GetGpsValid(&gps_current_info ,gps_recive_info.buffer);
            OpenGreenLed();
            gps_recive_succese = FALSE;
            gps_recive_time = 0;
            gps_recive_succese = FALSE;
            gps_recive_flag = FALSE;
            if(gps_current_info.valid == TRUE)
            {
                GPS_Analysis(&gps_current_info ,gps_recive_info.buffer);
                memset(gps_recive_info.buffer, 0, strlen((const char*)gps_recive_info.buffer));
                return;
            }
            memset(gps_recive_info.buffer, 0, strlen((const char*)gps_recive_info.buffer));
            Uart_EnableIrq(UARTCH1,UartRxIrq);
        }
    }
}

void GPS_Analysis(GPS_INFO *gpsx,uint8_t *buf)
{
//    Nmea_GPGSV_Analysis(gpsx,buf); //GPGSV 解析
//    Nmea_BDGSV_Analysis(gpsx,buf); //BDGSV 解析
//    Nmea_GNGSA_Analysis(gpsx,buf); //GNGSA 解析
    Nmea_GNGGA_Analysis(gpsx,buf); //GNGGA 解析
    Nmea_GNRMC_Analysis(gpsx,buf); //GNRMC 解析
    Nmea_GNVTG_Analysis(gpsx,buf); //GNVTG 解析
}
