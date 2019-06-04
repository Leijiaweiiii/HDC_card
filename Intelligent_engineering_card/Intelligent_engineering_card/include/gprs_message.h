#ifndef _MQTT_SEND_MESSAGE_H_
#define _MQTT_SEND_MESSAGE_H_
#include <stdint.h>
#include <stdarg.h>
#include "nb_iot_module.h"
#include "uart_deal.h"
#include "flash.h"
#include "gps.h"
#include "rgb.h"
#include "lpm.h"
#include "rtc.h"
#include "lpm_deal.h"

#define GPRS_MAX_BUFF       1152
#define GPS_MAX_BUFF        1024
#define MAX_APN_LEN         32
#define MAX_USER_LEN        32//用户名
#define MAX_PWD_LEN         32//用户密码
#define MAX_IP_LEN          32   //ip length max
#define MAX_SEND_AT_LEN     120  //max at length 
#define MAX_SEND_DATA_LEN   512  //max data len
#define MAX_SEND_AT_LINK    3
#define MAX_COMM_FRAME_DATA	192
#define SYS_MAXBUFF         128   //uart3 收发数据最长单个命令
#define IMEI_LEN            16
#define FLASH_SYSID_ADDR       0xF400
#define FLASH_TIDPWDUSR_ADDR   0xF200



extern uint16_t send_waittime;
extern uint8_t send_flag;
extern uint8_t flash_memory_id_flag;
extern uint8_t flash_memory_id_len;
extern uint8_t flash_memory_tidusrpwd_flag;
extern uint8_t flash_memory_tidusrpwd_len;
extern char *usr_pwd_base64;
extern uint8_t system_id[10];
extern uint8_t sim_card_flag;
extern uint32_t tidusrpwd_addr;
extern uint32_t sys_id_addr;
extern uint8_t tid_username_password[60];

typedef struct
{
	uint8_t buffer[GPS_MAX_BUFF + 2];
	uint16_t length;
	
}GPSResponseDistill;

typedef enum 
{
	SM_ON,
	SM_IDLE,
	SM_AT_SENDING,
	SM_AT_SEND_OK,
	SM_ATE0_SENDING,
	SM_ATE0_SEND_OK,
	SM_ATE1_SENDING,
	SM_ATE1_SEND_OK,
	SM_CGSN_SENDING,
	SM_CGSN_SEND_OK,
	SM_CCID_SENDING,
	SM_CCID_SEND_OK,
	SM_CSQ_SENDING,
	SM_CSQ_SEND_OK,
	SM_CPIN_SENDING,
	SM_CPIN_SEND_OK,
	SM_CREG_SENDING,
	SM_CREG_SEND_OK,
	SM_IMEI_SENDING,
	SM_IMEI_SEND_OK,
	SM_GPS_SENDING,
	SM_GPS_SEND_OK,
	SM_INIT_OK	
}AtInitStateMachineEnum;//at state machine

typedef enum
{
	SM_GPRS_XIIC_SENDING,
	SM_GPRS_XIIC_SEND_OK,
	SM_GPRS_XIIC_Q_SENDING,
	SM_GPRS_XIIC_Q_SEND_OK,
    
}GprsPPPStateEnum;

typedef enum
{
	SM_GPRS_HTTPSPARA_PORT_SENDING,
	SM_GPRS_HTTPSPARA_PORT_SEND_OK,
	SM_GPRS_HTTPSPARA_URL_SENDING,
    SM_GPRS_HTTPSPARA_URL_SEND_OK,
	SM_GPRS_HTTPSETUP_WAIT,
    SM_GPRS_HTTPSETUP_SENDING,
	SM_GPRS_HTTPSETUP_SEND_OK,
    SM_GPRS_HTTPACTION_SENDING,
	SM_GPRS_HTTPACTION_SEND_OK,
    SM_GPRS_HTTPSENDDATA_SENDING,
	SM_GPRS_HTTPSENDDATA_SEND_OK,
    SM_GPRS_DEAL_REV_DATA,
	SM_GPRS_HTTPSEND_SENDING,
	SM_GPRS_HTTPSEND_SEND_OK,
	SM_GPRS_HTTPCLOSE_SENDING,
	SM_GPRS_HTTPCLOSE_SEND_OK,
	
}GprsHttpStateEnum;

typedef enum
{
	SM_GPRS_MQTTCONNPARAM_SENDING,
	SM_GPRS_MQTTCONNPARAM_SEND_OK,
	SM_GPRS_MQTTCONN_SENDING,
    SM_GPRS_MQTTCONN_SEND_OK,
    SM_GPRS_MQTTSUB_SENDING,
	SM_GPRS_MQTTSUB_SEND_OK,
    SM_GPRS_MQTTPUB_SENDING,
	SM_GPRS_MQTTPUB_SEND_OK,
	SM_GPRS_MQTTRCVPUB_SENDING,
	SM_GPRS_MQTTRCVPUB_SEND_OK,
    SM_GPRS_MQTTSTATE_SENDING,
	SM_GPRS_MQTTSTATE_SEND_OK,
	SM_GPRS_MQTTDISCONN_SENDING,
	SM_GPRS_MQTTDISCONN_OK,
	
}GprsMqttStateEnum;

typedef enum
{
	GSM_IDLE,
	GSM_START,
	GSM_ALERING,//发信号
	GSM_RING,//
	GSM_NOSIM,//没有SIM卡
	GSM_CONNECT,
	GSM_SIGNAL_BAD,
	GSM_NORMAL,
	GSM_REP,
    GSM_SOS
}GSMLedStateMachineEnum;


typedef enum
{
    GPRS_SENDDATA_INIT,
    GPRS_REGISTER_ACTION,
    GPRS_REGISTER_SENDING,
	GPRS_REGISTER_SEND_OK,
	GPRS_REGISTER_OK,
	GPRS_CREDENTIALS_ACTION,
	GPRS_CREDENTIALS_SENDING,
	GPRS_CREDENTIALS_SEND_OK,
	GPRS_CREDENTIALS_OK,
	GPRS_UPDATA_ACTION,
	GPRS_UPDATA_SENDING,
	GPRS_UPDATA_SEND_OK,
	GPRS_UPDATA_OK,
    GPRS_GETUSERINFO_ACTION,
    GPRS_GETUSERINFO_SENDING,
    GPRS_GETUSERINFO_SEND_OK,
    GPRS_GETUSERINFO_OK,
    GPRS_CREATAGENT_ACTION,
    GPRS_CREATAGENT_OK,
    GPRS_CREATEXTERNID_ACTION,
    GPRS_CREATEXTERNID_OK,
    GPRS_SENDGPSINFO_ACTION,
    GPRS_SENDGPSINFO_OK,
    GPRS_SENDSOS_ACTION,
    GPRS_SENDSOS_OK,
    GPRS_DEALSOS_OK,
    
}GprsSendDataStatu;

typedef enum
{
    GPRS_SEND_LAT  =0,
    GPRS_SEND_LON,
    GPRS_SEND_SPEED,
    GPRS_SEND_HEADING,
    GPRS_SEND_ALTITUDE,
    GPRS_SEND_ACCURACY,
    GPRS_SEND_BATT
}GprsSendGpsDataEnum;

typedef struct
{
    GSMLedStateMachineEnum gsm_state_machine;
    AtInitStateMachineEnum at_init_state_machine;
    GprsPPPStateEnum gprs_ppp_state;
    GprsHttpStateEnum gprs_http_state;
    GprsMqttStateEnum gprs_mqtt_statu;
	uint8_t buffer[GPRS_MAX_BUFF + 2];
	uint16_t length;
    uint32_t overtime;//发送数据截至时间
    uint8_t  overtime_flag;
    
}AtResponseDistill;


typedef struct
{
	uint8_t buffer[MAX_SEND_DATA_LEN + 2];
	uint16_t length;
    uint32_t overtime;//发送数据截至时间
    uint8_t  overtime_flag;
    
}GprsSendDistill;

extern unsigned char g_register_state;

extern GprsSendDataStatu gprs_send_data_state;

extern GPSResponseDistill gps_recive_info;//接收数据

extern AtResponseDistill at_response_parser;

extern GprsSendDistill gprs_send_parser;

extern GprsSendGpsDataEnum gprs_send_gps_data;

extern uint8_t creat_system_id_flag;//创建设备ID完成

en_result_t AtInit(AtInitStateMachineEnum at_init_state_machine);

void GprsHttpSendInfo(void);

void MqttSendInfo(void);

en_result_t ReadFlashData(void);

void flash_memory_data(uint8_t argv_num, ...);
#endif

