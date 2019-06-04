#ifndef __GPS__H__
#define __GPS__H__

#include "stdint.h"

typedef	unsigned char		BYTE;
typedef	unsigned char		BOOL;

typedef enum
{
    GPS_INFO_RECIVE_INIT,
    GPS_INFO_RECIVE_START,
    GPS_INFO_RECIVEING,
    GPS_INFO_RECIVED,
	
}GPS_INFO_RECIVE;

typedef enum 
{
    RECIVE_GPS_START = 0 ,
	RECIVE_GPS_CR,
	RECIVE_GPS_LF,
	RECIVE_GPS_END  
	
}GPS_RECIVE_STATUS;

typedef struct
{
	int8_t sleepHour;
	int8_t sleepmin;
    int8_t sleepSecend;
} DevNV_GPS_Sleep;//GPS睡眠状态

typedef struct
{
	uint8_t     NS;  //北纬南纬
	uint8_t     EW;  //东经西经
	double latitude; //经度
	double longitude;//纬度
	uint32_t range;
} DevNV_GPS_Barrier;//设置位置围栏

typedef struct
{
	BYTE Baud_id;   //波特率选项
	uint32_t openTimes;//打开时间
	BYTE openStatus;   //打开状态
	BYTE takeOFFstatus;//获取关闭状态
	
} BandParmas;

typedef struct{
    int year; 
    int month; 
    int day;
    int hour;
    int minute;
    uint8_t second;
} DATE_TIME;//获取的日期

typedef struct{
    //uint8_t   fixmode;          //模式
    uint8_t	  valid;            //有效
    double    latitude;         //经度
    double    longitude;        //纬度
    //float     latitude_gitude;  //度
    //float     latitude_Cent;    //分
    //float     latitude_Second;  //秒
    //float     longitude_Degree; //度
    //float     longitude_Cent;   //分
    //float     longitude_Second; //秒
    double    heading;    //航向
    float     speed;      //速度
    float     direction;  //航向
    float     height;     //海拔高度
    int       satellite;  //卫星
    uint8_t   NS;         //北纬南纬
    uint8_t   EW;         //东经西经
    double    accuracy;     //精度
    DATE_TIME utc_time;   //
} GPS_INFO;//GPS信息       

typedef struct
{
	uint16_t sos; //启动序列，固定为 0XA0A1
	uint16_t PL; //有效数据长度 0X0004；
	uint8_t id; //ID，固定为 0X05
	uint8_t com_port; //COM 口，固定为 0X00，即 COM1
	uint8_t Baud_id;
	//波特率（0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600）
	uint8_t Attributes;
	//配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
	uint8_t CS; //校验值
	uint16_t end; //结束符:0X0D0A
	
}SkyTra_baudrate;

typedef struct
{
    uint16_t sos; //启动序列，固定为 0XA0A1
    uint16_t PL; //有效数据长度 0X0003；
    uint8_t id; //ID，固定为 0X0E
    uint8_t rate; //取值范围:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
    uint8_t Attributes;
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    uint8_t CS; //校验值
    uint16_t end; //结束符:0X0D0A
}SkyTra_PosRate;

typedef struct
{
    uint16_t sos; //启动序列，固定为 0XA0A1
    uint16_t PL; //有效数据长度 0X0007；
    uint8_t id; //ID，固定为 0X65
    uint8_t Sub_ID; //0X01
    uint32_t width; //1~100000(us)
    uint8_t Attributes;
    //配置数据保存位置 ,0 保存到 SRAM，1 保存到 SRAM&FLASH，2 临时保存
    uint8_t CS; //校验值
    uint16_t end; //结束符:0X0D0A
}SkyTra_pps_width;

extern GPS_INFO gps_current_info;       //当前位置信息
extern GPS_INFO_RECIVE gps_info_recive_statue;//接收时间是否结束

extern GPS_RECIVE_STATUS gps_recive_status;

extern uint8_t gps_info_valid;
extern uint8_t gps_info_send_statu;
extern DevNV_GPS_Sleep gps_sleep;
uint8_t SkyTra_Cfg_Prt(uint32_t baud_id);
uint8_t SkyTra_Cfg_Tp(uint32_t width);
uint8_t Nmea_Comma_Pos(uint8_t *buf,uint8_t cx);
uint32_t Nmea_Pow(uint8_t m,uint8_t n);
int Nmea_Str2num(uint8_t *buf,uint8_t*dx);
void Nmea_GPGSV_Analysis(GPS_INFO *gpsx,uint8_t *buf);
void Nmea_BDGSV_Analysis(GPS_INFO *gpsx,uint8_t *buf);
void Nmea_GNGGA_Analysis(GPS_INFO *gpsx, uint8_t *buf);
void Nmea_GNGSA_Analysis(GPS_INFO *gpsx, uint8_t *buf);
void Nmea_GNRMC_Analysis(GPS_INFO *gpsx,uint8_t *buf);
void Nmea_GNVTG_Analysis(GPS_INFO *gpsx,uint8_t *buf);
void GPS_Analysis(GPS_INFO *gpsx,uint8_t *buf);
void OpenGps(void);
void CloseGps(void);
void GpsInit(void);
void CheckGpsInfoValue(void);
void GetGpsValid(GPS_INFO *gpsx,uint8_t *buf);
#endif
