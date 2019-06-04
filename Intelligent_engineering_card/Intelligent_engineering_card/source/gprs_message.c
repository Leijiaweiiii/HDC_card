#include "gprs_message.h"

#define SET_PPP_MAX                3    //建ppp连接失败最高次数
#define QUERY_SET_PPP_MAX          20   //查询ip地址最高次数
#define QUERY_SIGNAL_INTENSITY_MAX 40   //查询信号强度最高次数
#define QUERY_SIM_CARD_MAX         10   //查询SIM卡状态最高次数
#define MQTT_SEND_COUNT_MAX        60   //MQTT发送连续错误次数
#define RECIVE_OVERTIMER           30000       
#define NET_REG_COUNT_MAX          5
#define AT_SEND_MAX                10

AtResponseDistill at_response_parser = {
                                        GSM_IDLE,
                                        SM_AT_SENDING,
                                        SM_GPRS_XIIC_SENDING,
                                        SM_GPRS_HTTPSPARA_URL_SENDING,
                                        SM_GPRS_MQTTCONNPARAM_SENDING,
                                         {0}, 
                                         0, 
                                         0,
                                         0,
                                       };

                                       
GprsSendDistill gprs_send_parser = {
                                    {0},
                                    0,
                                    FALSE
                                   };
uint8_t imei_data[IMEI_LEN]="869060030502128";

GprsSendDataStatu gprs_send_data_state = GPRS_REGISTER_ACTION;
                             

GprsSendGpsDataEnum gprs_send_gps_data = GPRS_SEND_LAT;
                                   
//uint8_t tid_username_password[45] = {"pjjh/"};
                                   
uint8_t tenantid[10] = {"pjjh"};
uint8_t username[30] = {"device_869060030502128"};
uint8_t password[15] = {"8JLB(k7vrI"};
                                       
uint8_t system_id[10] = {"36711"};
char base64_char[60] = {"cGpqaC9kZXZpY2VfODY5MDYwMDMwNTAyMTI4OjhKTEIoazd2ckk="};
char *usr_pwd_base64 = base64_char;
static const char *alpha_base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
uint8_t tid_username_password[60] = {0};
char time[40] = {"2019-04-19T18:35:14.000+08:00"};
uint8_t creat_system_id_flag = 0;

uint8_t flash_memory_id_flag = 0;
uint8_t flash_memory_tidusrpwd_flag = 0;
uint8_t flash_memory_id_len = 0;
uint8_t flash_memory_tidusrpwd_len = 0;
uint8_t gps_info_valid = 0;
uint8_t gps_info_send_statu = 0;
uint8_t sim_card_flag = 0;
uint32_t tidusrpwd_addr = FLASH_TIDPWDUSR_ADDR;
uint32_t sys_id_addr = FLASH_SYSID_ADDR;
uint32_t creat_sys_id_addr = 0;
//static char *send_gps_frament[7] = {
//                                     "c8y_lat",
//                                     "c8y_lon,lon",
//                                     "c8y_speed",
//                                     "c8y_heading",
//                                     "c8y_altitude",
//                                     "c8y_accuracy",
//                                     "c8y_batt"
//                                   };

//static char *send_gps_series[7] = {
//                                    "lat",
//                                    "lon",
//                                    "speed",
//                                    "heading",
//                                    "altitude",
//                                    "accuracy",
//                                    "batt"
//                                  };
//static char *send_gps_format[7] = {
//                                    "%.6f",
//                                    "%.6f",
//                                    "%.2f",
//                                    "%.2f",
//                                    "%.2f",
//                                    "%.2f",
//                                    "%.2f"
//                                  };


/*******************************************************************************
 ** \brief  将ASCII码转换位Base64编码
 **
 ** \param [in] buf: ASCII字符串
 **        size: ASCII字符串长度
 **        base64Char: 返回的Base64编码字符串
 **        
 **
 ** \retval 字符串 
 ******************************************************************************/
static char *Encode(const char *buf, const int size, char *base64Char) 
{
    int a = 0;
    int i = 0;
    while (i < size) {
        char b0 = buf[i++];
        char b1 = (i < size) ? buf[i++] : 0;
        char b2 = (i < size) ? buf[i++] : 0;
         
        int int63 = 0x3F; //  00111111
        int int255 = 0xFF; // 11111111
        base64Char[a++] = alpha_base[(b0 >> 2) & int63];
        base64Char[a++] = alpha_base[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
        base64Char[a++] = alpha_base[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
        base64Char[a++] = alpha_base[b2 & int63];
    }
    switch (size % 3) {
        case 1:
            base64Char[--a] = '=';
        case 2:
            base64Char[--a] = '=';
    }
    return base64Char;
}

void flash_memory_data(uint8_t argv_num, ...)
{
    va_list args;
    va_start(args, argv_num);
    uint8_t i = 1;
    uint8_t flash_flag = 0;
    uint8_t operat_flag;
    uint8_t flash_len;
    uint8_t data[60];
    uint32_t cur_addr = va_arg(args, uint32_t);
    //printf("***************cur_addr = %0x\r\n", cur_addr);
    uint32_t prev_addr = cur_addr;
    uint32_t next_addr = cur_addr + 1;
    Flash_Init(FlashInt, 12, TRUE);
    Flash_SectorErase(cur_addr);
    while(i < argv_num)
    {
        switch(i%3)
        {
            case 0:
                cur_addr = prev_addr;//第一个地址存Flag
                operat_flag = (uint8_t)va_arg(args, uint32_t);
                //printf("***************operat_flag = %d*****************\r\n", operat_flag);
                if(operat_flag == 1 || operat_flag == 2)
                {
                    flash_flag = (flash_flag << 1) | 1;
                    //flash_flag = 0x07;
                }
                if(i == 9)FlashWriteData(&flash_flag, &cur_addr, sizeof(flash_flag));
                //printf("***************flash_flag = 0x%x*****************\r\n", flash_flag);   
                break;
            case 1:
                
                flash_len = (uint8_t)va_arg(args, uint32_t);
                //printf("***************flash_len = %d\r\n", flash_len);
                FlashWriteData(&flash_len, &next_addr, sizeof(flash_len));//1为flag长度
                break;
            case 2:
                strcpy((char*)data, (const char*)va_arg(args, int8_t*));
                //printf("***************data = %s\r\n", data);
                FlashWriteData(data, &next_addr, flash_len);
                break;
            default:
                break;
        }
        i++;
    }
    va_end(args);

    return;
}

en_result_t ReadFlashData(void)
{
    uint8_t i = 0;
    int8_t j = 0;
    BOOL tid_flag = TRUE;
    BOOL username_flag = TRUE;
    BOOL password_flag = TRUE;
    uint32_t flash_first_addr = FLASH_TIDPWDUSR_ADDR;
    uint8_t flash_flag;
    FlashReadData(&flash_flag, &flash_first_addr, sizeof(flash_flag));
    //printf("****************0x%d*******************\r\n", flash_flag);
    if((flash_flag & 0x01) == 0x01)
    {
        flash_memory_tidusrpwd_flag = 1;
        FlashReadData(&flash_memory_tidusrpwd_len, &flash_first_addr, sizeof(flash_memory_tidusrpwd_len));
        FlashReadData(tid_username_password, &flash_first_addr, flash_memory_tidusrpwd_len);
        usr_pwd_base64 = Encode((char *)tid_username_password, strlen((const char *)tid_username_password), (char *)base64_char);
        for(i=0; i < flash_memory_tidusrpwd_len; i++)
        {
            if(tid_flag)
            {
                if(tid_username_password[i] != '/')
                    tenantid[j] = tid_username_password[i];
                else
                {
                    tid_flag = FALSE;
                    j = -1;
                }
            }
            else if(username_flag)
            {
                if(tid_username_password[i] != ':')
                    username[j] = tid_username_password[i];
                else
                {
                    username_flag = FALSE;
                    j = -1;
                }
            }
            else if(password_flag)
            {
                if(tid_username_password[i] != '\0')
                    password[j] = tid_username_password[i];
                else
                {
                    password_flag = FALSE;
                    j = -1;
                }
            }
			j++;
        }
    }
    else
    {
        flash_memory_tidusrpwd_flag = 0;
    }
    if((flash_flag & 0x02) == 0x02)
    {
        flash_memory_id_flag = 1;
        FlashReadData(&flash_memory_id_len, &flash_first_addr, sizeof(flash_memory_id_len));
        FlashReadData(system_id, &flash_first_addr, flash_memory_id_len);

    }
    else
    {
        flash_memory_id_flag = 0;
        
    }
    if((flash_flag & 0x04) == 0x04)
    {
        creat_system_id_flag = 1;
    }
    else
    {
        creat_system_id_flag = 0;
    }
//    printf("******************tenantid :%s***********************\n",tenantid);
//    printf("******************username :%s***********************\n",username);
//    printf("******************password :%s***********************\n",password);
//    printf("******************system_id :%s***********************\n",system_id);
    return Ok;
}
//uint32_t flash_memory_data(uint8_t *data, uint8_t data_len, uint8_t flag, uint32_t cur_addr)
//{
//    uint32_t prev_addr = cur_addr;
//    uint32_t next_addr = 0;
//    Flash_Init(FlashInt, 12, TRUE);
//    Flash_SectorErase(cur_addr);
//    ++cur_addr;//第二个地址存长度
//    FlashWriteData(&data_len, &cur_addr, sizeof(data_len));//1为flag长度
//    FlashWriteData(data, &cur_addr, strlen((const char *)data));
//    next_addr = cur_addr;
//    cur_addr = prev_addr;//第一个地址存Flag
//    FlashWriteData(&flag, &cur_addr, sizeof(flag));
//    
//    return next_addr;
//}

//en_result_t ReadFlashData(void)
//{
//    uint8_t tid_username_password[60] = {0};
//    uint8_t i = 0;
//    int8_t j = 0;
//    BOOL tid_flag = TRUE;
//    BOOL username_flag = TRUE;
//    BOOL password_flag = TRUE;
//    tidusrpwd_addr = FLASH_TIDPWDUSR_ADDR;
//    sys_id_addr = FLASH_SYSID_ADDR;
//    FlashReadData(&flash_memory_id_flag, &sys_id_addr, sizeof(flash_memory_id_flag));
//    if(flash_memory_id_flag == 1)
//    {
//        FlashReadData(&flash_memory_id_len, &sys_id_addr, sizeof(flash_memory_id_len));
//        FlashReadData(system_id, &sys_id_addr, flash_memory_id_len);
//    }
//    else
//    {
//        flash_memory_id_flag = 0;
//    }
//    FlashReadData(&flash_memory_tidusrpwd_flag, &tidusrpwd_addr, sizeof(flash_memory_tidusrpwd_flag));
//    if(flash_memory_tidusrpwd_flag == 1)
//    {
//        FlashReadData(&flash_memory_tidusrpwd_len, &tidusrpwd_addr, sizeof(flash_memory_tidusrpwd_len));
//        FlashReadData(tid_username_password, &tidusrpwd_addr, flash_memory_tidusrpwd_len);
//        usr_pwd_base64 = Encode((char *)tid_username_password, strlen((const char *)tid_username_password), (char *)base64_char);
//        for(i=0; i < flash_memory_tidusrpwd_len; i++)
//        {
//            if(tid_flag)
//            {
//                if(tid_username_password[i] != '/')
//                    tenantid[j] = tid_username_password[i];
//                else
//                {
//                    tid_flag = FALSE;
//                    j = -1;
//                }
//            }
//            else if(username_flag)
//            {
//                if(tid_username_password[i] != ':')
//                    username[j] = tid_username_password[i];
//                else
//                {
//                    username_flag = FALSE;
//                    j = -1;
//                }
//            }
//            else if(password_flag)
//            {
//                if(tid_username_password[i] != '\0')
//                    password[j] = tid_username_password[i];
//                else
//                {
//                    password_flag = FALSE;
//                    j = -1;
//                }
//            }
//			j++;
//        }
//    }
//    else
//    {
//        flash_memory_tidusrpwd_flag = 0;
//    }
//	
//    return Ok;
//}


uint8_t *GetTidUsernamePSW(void)
{
    static uint8_t tid_username_password[60] = {0};
    sprintf((char *)tid_username_password, "%s/%s:%s", (const char*)tenantid, (const char*)username, (const char*)password);
    return tid_username_password;
}


/*******************************************************************************
** \brief  获取用户名以及密码将用户名密码存入flash
 **
 ** \param [in] buf: ASCII字符串
 **        size: ASCII字符串长度
 **        base64Char: 返回的Base64编码字符串
 **        
 **
 ** \retval 字符串 
 ******************************************************************************/
en_result_t GetIdUsrpwdToFlash(void)
{
    uint8_t *temporary_data;
    uint8_t marks_count = 0;
    uint8_t i = 5;
    
    temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "\"tenantId\"");//平台id
    
    while(marks_count < 4)
    {
        if(*temporary_data == '"')
        {
            marks_count++;
        }
        if((marks_count == 3) && (*temporary_data != '"'))
        {
            tenantid[i] = *temporary_data;
            i++;
        }
        temporary_data++;
            
    }
    i = 0;
    marks_count = 0;
    
    temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "\"username\"");//获取密码
    
    while(marks_count < 4)
    {
        if(*temporary_data == '"')
        {
            marks_count++;
        }
        if((marks_count == 3) && (*temporary_data != '"'))
        {
            username[i] = *temporary_data;
            i++;
        }
        temporary_data++;
            
    }
    i = 0;
    marks_count = 0;
    temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "\"password\"");//获取用户名 
    
    while(marks_count < 4)//用while因为不知道数据长度
    {
        if(*temporary_data == '"')
        {
            marks_count++;
        }
        if((marks_count == 3) && (*temporary_data != '"'))
        {
            password[i] = *temporary_data;
            i++;
        }
        temporary_data ++;
            
    }
    
    i = 0;
    memcpy(tid_username_password, GetTidUsernamePSW(), strlen((const char*)GetTidUsernamePSW()));
    flash_memory_tidusrpwd_flag = 1;
    flash_memory_tidusrpwd_len = strlen((const char *)tid_username_password);
    flash_memory_data(10, tidusrpwd_addr, flash_memory_tidusrpwd_len, tid_username_password, flash_memory_tidusrpwd_flag,
                                         flash_memory_id_len, system_id, flash_memory_id_flag,
                                         0, 0, creat_system_id_flag);
    gprs_send_data_state = GPRS_GETUSERINFO_OK;
    usr_pwd_base64 = Encode((char *)tid_username_password, strlen((const char *)tid_username_password), (char *)base64_char);
    
    return Ok;
}
en_result_t GetSystemIdToFlash(void)
{
    uint8_t *temporary_data;
    uint8_t marks_count = 0;
    uint8_t i = 0;
    
    temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "\"id\"");//系统ID
    while(marks_count < 4)
    {
        if((marks_count == 3) && (*temporary_data != '"'))
        {
            system_id[i] = *temporary_data;
            i++;
        }
        if(*temporary_data == '"')
        {
            marks_count++;
        }
        temporary_data++;      
    }
    flash_memory_id_flag = 1;
    flash_memory_id_len = strlen((const char*)system_id);
    flash_memory_data(10, tidusrpwd_addr, flash_memory_tidusrpwd_len, tid_username_password, flash_memory_tidusrpwd_flag,
                                         flash_memory_id_len, system_id, flash_memory_id_flag,
                                         0, 0, creat_system_id_flag);
    return Ok;
}


//static en_result_t find_str(uint8_t des_str[])
//{
//    if (strstr((const char *)at_response_parser.buffer, (const char *)des_str) != NULL)
//    {
//        return Ok;
//    }
//    else
//    {
//        return Error;
//    }
//}

 /*******************************************************************************
** \brief  需要上传HTTP的数据
 **
 ** \param [in] u8Idx通道号
 **        data_buf 串口接收到的数据
 **
 ** \retval Error 获取成功
 **\retval OK 获取失败
 *******************************************************************************/
uint8_t *GetSendInfo(GprsSendDataStatu gprs_send_data_state)
{
    uint8_t json_data[255];
    memset(gprs_send_parser.buffer, 0, gprs_send_parser.length);
    /************************1.注册设备*************************
    ***********************************************************/
    if(gprs_send_data_state == GPRS_REGISTER_ACTION)
    {
        sprintf((char *)json_data, "{\"id\":\"%s\"}", imei_data);
        sprintf((char *)gprs_send_parser.buffer, 
        "POST /devicecontrol/newDeviceRequests HTTP/1.1\r\n"
        "HOST: pjjh.lnltiot.com\r\n"
        "Content-Type: application/json\r\n"
        "Accept: application/vnd.com.nsn.cumulocity.newDeviceRequest+json;\r\n"
        "Authorization: Basic ZGV2aWNlOmRldmljZUAxMjM=\r\n"  
        "connection: close\r\n"
        "Content-Length: %d\r\n\r\n"
        "%s", strlen((const char*)json_data), json_data);
    }
    /***********************2.创建设备凭证**********************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_CREDENTIALS_ACTION)
    {
        sprintf((char *)json_data, "{\"id\":\"%s\"}", imei_data);
        sprintf((char *)gprs_send_parser.buffer, 
        "POST /devicecontrol/deviceCredentials HTTP/1.1\r\n"                          
        "HOST: pjjh.lnltiot.com\r\n"                     
        "Content-Type: application/json\r\n"
        "Authorization: Basic bWFuYWdlbWVudC9kZXZpY2Vib290c3RyYXA6RmhkdDFiYjFm\r\n"
        "connection: close\r\n"
        "Content-Length: %d\r\n\r\n"
        "%s" , strlen((const char*)json_data), json_data);
    }
    /*********************3.创建设备凭证***********************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_UPDATA_ACTION)
    {
        sprintf((char *)gprs_send_parser.buffer, 
        "PUT /devicecontrol/newDeviceRequests/%s HTTP/1.1\r\n"
        "HOST: pjjh.lnltiot.com\r\n"
        "Content-Type: application/json\r\n"
        "Authorization: Basic ZGV2aWNlOmRldmljZUAxMjM=\r\n"
        "connection: close\r\n"
        "Content-Length: 22\r\n\r\n"
        "{\"status\": \"ACCEPTED\"}", imei_data);
    }
    /**********************4.获取用户名密码********************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_GETUSERINFO_ACTION)
    {
        sprintf((char *)json_data, "{\"id\":\"%s\"}", imei_data);
        sprintf((char *)gprs_send_parser.buffer,
        "POST /devicecontrol/deviceCredentials HTTP/1.1"
        "\r\nHOST: pjjh.lnltiot.com"
        "\r\nContent-Type: application/json"
        "\r\nAuthorization: Basic bWFuYWdlbWVudC9kZXZpY2Vib290c3RyYXA6RmhkdDFiYjFm"
        "\r\nconnection: close"
        "\r\nContent-Length: %d\r\n\r\n"
        "%s", strlen((const char*)json_data), json_data);
    }
    /************************5.创建Agent************************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_CREATAGENT_ACTION)
    {
        sprintf((char *)json_data, "{\"name\":\"%s\",\"com_cumulocity_model_Agent\":{},\"c8y_IsDevice\":{}}", imei_data);
        sprintf((char *)gprs_send_parser.buffer,
        "POST /inventory/managedObjects HTTP/1.1\r\n"
        "HOST: pjjh.lnltiot.com\r\n"
        "Content-Type: application/json\r\n"
        "Accept: application/json\r\n"
        "Authorization: Basic %s\r\n"
        "connection: close\r\n"
        "Content-Length: %d\r\n\r\n"
        "%s", usr_pwd_base64, strlen((const char*)json_data), json_data);
    }
    /************************6.创建外部id***********************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_CREATEXTERNID_ACTION)
    {
        sprintf((char *)json_data, "{\"externalId\":\"%s\",\"type\":\"c8y_Serial\"}", imei_data);
        sprintf((char *)gprs_send_parser.buffer,
        "POST /identity/globalIds/%s/externalIds HTTP/1.1\r\n"
        "HOST: pjjh.lnltiot.com\r\n"
        "Content-Type: application/vnd.com.nsn.cumulocity.externalId+json\r\n"
        "Accept: application/vnd.com.nsn.cumulocity.externalId+json\r\n"
        "Authorization: Basic %s\r\n"
        "connection: close\r\n"
        "Content-Length: %d\r\n\r\n"
        "%s" , system_id, base64_char, strlen((const char*)json_data), json_data);
    }
    /************************7.发送GPS信息********************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_SENDGPSINFO_ACTION)
    {
        while(IsEmpty(&gps_queue) == FALSE){;}
        gps_current_info = DeQueue(&gps_queue);
        //时间拼接
        sprintf((char *)time, 
        "%04d-%02d-%02dT%02d:%02d:%.3f+08:00",
        gps_current_info.utc_time.year,
        gps_current_info.utc_time.month,
        gps_current_info.utc_time.day,
        (gps_current_info.utc_time.hour + 8),
        gps_current_info.utc_time.minute,
        (double)gps_current_info.utc_time.second);
        //信息拼接
        sprintf((char *)gprs_send_parser.buffer,"AT+MQTTPUB=0,1,\"s/us\","
        "\"200,lat,lat,%.6f,,%s\n"
        "200,lon,lon,%.6f,,%s\n"
        "200,s,s,%.2f,,%s\n"
        "200,h,h,%.2f,,%s\n"
        "200,alt,alt,%.2f,,%s\n"
        "200,acy,acy,%.2f,,%s\n"
        "200,bat,bat,%.2f,,%s\"\r",
        gps_current_info.latitude,time,
        gps_current_info.longitude,time,  
        gps_current_info.speed,time,
        gps_current_info.heading,time,
        gps_current_info.height,time,
        gps_current_info.accuracy,time,
        1.00,time
        );
//        sprintf((char *)gprs_send_parser.buffer,"AT+MQTTPUB=0,1,\"s/us\","
//        "\"200,c8y_lat,lat,%.6f,,%s\n"
//        "200,c8y_lon,lon,%.6f,,%s\n"
//        "200,c8y_speed,speed,%.2f,,%s\n"
//        "200,c8y_heading,heading,%.2f,,%s\n"
//        "200,c8y_altitude,altitude,%.2f,,%s\n"
//        "200,c8y_accuracy,accuracy,%.2f,,%s\n"
//        "200,c8y_batt,batt,%.2f,,%s\"\r",
//        gps_current_info.latitude,time,
//        gps_current_info.longitude,time,  
//        gps_current_info.speed,time,
//        gps_current_info.heading,time,
//        gps_current_info.height,time,
//        gps_current_info.accuracy,time,
//        1.00,time
//        );
    }
    /**********************8.求救信号**************************
    ***********************************************************/
    else if(gprs_send_data_state == GPRS_SENDSOS_ACTION)
    {
        sprintf((char *)gprs_send_parser.buffer, "AT+MQTTPUB=0,1,\"s/us\","
        "\"301,AlarmSOS,S.O.S HELP!,,%s\"\r",
        time);
    }
#ifdef HTTP_SEND //使用HTTP发送
//    /************************7.发送GPS信息********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_SENDGPSINFO_ACTION)
//    {
//        //时间拼接
//        sprintf((char *)time, 
//        "%04d-%02d-%02dT%02d:%02d:%.3f+08:00",
//        gps_current_info.utc_time.year,
//        gps_current_info.utc_time.month,
//        gps_current_info.utc_time.day,
//        gps_current_info.utc_time.hour,
//        gps_current_info.utc_time.minute,
//        (double)gps_current_info.utc_time.second);
//        //发送JSON信息拼接
//        sprintf((char *)send_body, 
//        "{\"measurements\":"
//           "["
//              "{\"c8y_lat\":{\"lat\":{\"value\":%.6f,\"unit\":\"°\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_lat\"},"
//              "{\"c8y_lon\":{\"lon\":{\"value\":%.6f,\"unit\":\"°\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_lon\"},"
//              "{\"c8y_speed\":{\"speed\":{\"value\":%.2f,\"unit\":\"km/h\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_speed\"},"
//              "{\"c8y_heading\":{\"heading\":{\"value\":%.2f,\"unit\":\"°\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_heading\"},"
//              "{\"c8y_altitude\":{\"altitude\":{\"value\":%.2f,\"unit\":\"°\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_altitude\"},"
//              "{\"c8y_accuracy\":{\"accuracy\":{\"value\":%.2f,\"unit\":\"\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_accuracy\"},"
//              "{\"c8y_batt\":{\"batt\":{\"value\":%.2f,\"unit\":\"\"}},\"time\":\"%s\",\"source\":{\"id\":\"%s\"},\"type\":\"c8y_batt\"}"
//           "]"
//        "}", gps_current_info.latitude,time,system_id,
//             gps_current_info.longitude,time,system_id,  
//             gps_current_info.speed,time,system_id,
//             gps_current_info.heading,time,system_id,
//             gps_current_info.height,time,system_id,
//             gps_current_info.accuracy,time,system_id,
//             1.00,time,system_id);
//        //发送体拼接
//        sprintf((char *)gprs_send_parser.buffer,
//        "POST /measurement/measurements HTTP/1.1\r\n"
//        "Host: pjjh.lnltiot.com\r\n"
//        "Content-Type: application/json\r\n"
//        "Accept: application/vnd.com.nsn.cumulocity.measurementCollection+json\r\n"
//        "Authorization: Basic %s\r\n"
//        "connection: close\r\n"
//        "Content-Length: %d\r\n\r\n"
//        "%s",usr_pwd_base64 ,strlen((char *)send_body)- 4, send_body);
//    }
//    /**********************8.求救信号**************************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_SENDSOS_ACTION)
//    {
//        sprintf((char *)gprs_send_parser.buffer,
//        "POST /alarm/alarms HTTP/1.1"
//        "\r\nHOST: pjjh.lnltiot.com"
//        "\r\nContent-Type: application/json"
//        "\r\nAccept: application/vnd.com.nsn.cumulocity.alarm+json"
//        "\r\nAuthorization: Basic ZGV2aWNlOmRldmljZUAxMjM="
//        "\r\nconnection: close"
//        "\r\nContent-Length: 147\r\n\r\n"
//        "{\"source\": {\"id\": \"%s\" },\"type\": \"AlarmSOS\",\"text\": \"S.O.S HELP!\",\"severity\": \"MAJOR\",\"status\": \"ACTIVE\",\"time\":\"%s\"}", 
//        system_id, time);
//    }
#endif
    gprs_send_parser.length = strlen((const char *)gprs_send_parser.buffer);
    return gprs_send_parser.buffer;

}


void HttpSendData(void)
{
    uint8_t http_action_data_buff[23] = {0};
    uint8_t http_action_data[17] = "AT+HTTPACTION=99,";
    if(flash_memory_tidusrpwd_flag == 0)
    {
        if(gprs_send_data_state == GPRS_REGISTER_ACTION)
        {
            GetSendInfo(gprs_send_data_state);
        }
        else if(gprs_send_data_state == GPRS_CREDENTIALS_ACTION)//2.创建设备凭证
        {
            GetSendInfo(gprs_send_data_state);
        }
        else if(gprs_send_data_state == GPRS_UPDATA_ACTION)//3.创建设备凭证
        {
            GetSendInfo(gprs_send_data_state);
        }
        else if(gprs_send_data_state == GPRS_GETUSERINFO_ACTION)//4.获取用户名密码
        {
            GetSendInfo(gprs_send_data_state);
        }
    }
    else if(flash_memory_tidusrpwd_flag == 1)
    {
        flash_memory_tidusrpwd_flag = 2;
        gprs_send_data_state = GPRS_CREATAGENT_ACTION;
    }
    if(flash_memory_id_flag == 0)
    {
        if(gprs_send_data_state == GPRS_CREATAGENT_ACTION )//5.创建Agent
        {
            GetSendInfo(gprs_send_data_state);
        }
    }
    else if(flash_memory_id_flag == 1)
    {
        gprs_send_data_state = GPRS_CREATEXTERNID_ACTION;
        flash_memory_id_flag = 2;
    }
    if(gprs_send_data_state == GPRS_SENDSOS_ACTION)//发送求救
    {
        GetSendInfo(gprs_send_data_state);
        sos_key = 2;
    }
    else if(gprs_send_data_state == GPRS_CREATEXTERNID_ACTION)//6.创建外部id
    {
        GetSendInfo(gprs_send_data_state);
    }
    else if((gprs_send_data_state == GPRS_CREATEXTERNID_OK)||
            (gprs_send_data_state == GPRS_SENDGPSINFO_ACTION)||
            (gprs_send_data_state == GPRS_DEALSOS_OK))//7.发送定位数据
    {
        gprs_send_data_state = GPRS_SENDGPSINFO_ACTION;
        GetSendInfo(gprs_send_data_state);
    }
    sprintf((char *)http_action_data_buff, "%s%d\r", http_action_data, gprs_send_parser.length);
    Send_Command_Data(UARTCH0, (const uint8_t *)http_action_data_buff);
    
    return ;
}     


//处理HTTP回显
void ReciveDataDeal(void)
{
    if (gprs_send_data_state == GPRS_REGISTER_ACTION)//注册时回显
    {
        if ((strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)||
            (strstr((const char *)at_response_parser.buffer, "already exists") != NULL))
        {
            gprs_send_data_state = GPRS_CREDENTIALS_ACTION;
        }
    }
    else if (gprs_send_data_state == GPRS_CREDENTIALS_ACTION)//创建设备凭证
    {
        if (strstr((const char *)at_response_parser.buffer, "PENDING_ACCEPTANCE") != NULL)
        {
            gprs_send_data_state = GPRS_UPDATA_ACTION;
        }
    }
    else if (gprs_send_data_state == GPRS_UPDATA_ACTION)//更新设备凭证
    {
        if (strstr((const char *)at_response_parser.buffer, "200 OK") != NULL)
        {
            gprs_send_data_state = GPRS_GETUSERINFO_ACTION;
        }
    }
    else if (gprs_send_data_state == GPRS_GETUSERINFO_ACTION)//获取用户及密码
    {
        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
        {
            gprs_send_data_state = GPRS_CREATAGENT_ACTION;
            GetIdUsrpwdToFlash();//把获取到的用户信息保存到flash
        }
    }
    else if (gprs_send_data_state == GPRS_CREATAGENT_ACTION)//创建代理
    {
        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
        {
           gprs_send_data_state = GPRS_CREATEXTERNID_ACTION;
           GetSystemIdToFlash();
        }
    }
    else if (gprs_send_data_state == GPRS_CREATEXTERNID_ACTION)//创建外部ID
    {
        if ((strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)||
            (strstr((const char *)at_response_parser.buffer, "already") != NULL))
        {
            gprs_send_data_state = GPRS_CREATEXTERNID_OK;
            creat_system_id_flag = 1;
            flash_memory_data(10, tidusrpwd_addr, flash_memory_tidusrpwd_len, tid_username_password, flash_memory_tidusrpwd_flag,
                                                 flash_memory_id_len, system_id, flash_memory_id_flag,
                                                 0, 0, creat_system_id_flag);
      
        }
    }
//    if(creat_system_id_flag == 1)
//    {
//        if(sos_key == 1) gprs_send_data_state = GPRS_SENDSOS_ACTION;
//    }
//    else if (gprs_send_data_state == GPRS_SENDSOS_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "ACTIVE") != NULL)
//        {
//            gprs_send_data_state = GPRS_SENDSOS_OK;
//            sos_key = 0;
//        }
//    }
//    else if (gprs_send_data_state == GPRS_SENDGPSINFO_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
//        {
//            //gprs_send_data_state = GPRS_SENDGPSINFO_OK;
//        }
//        if ((strstr((const char *)at_response_parser.buffer, "500 Server Error")))
//        {
//            //gprs_send_data_state = GPRS_SENDGPSINFO_OK;
//        }
//    }
//    if ((strstr((const char *)at_response_parser.buffer, "ERROR"))||
//        (strstr((const char *)at_response_parser.buffer, "+HTTPACTION: OPERATION EXPIRED"))||
//        (strstr((const char *)at_response_parser.buffer, "+HTTPCLOSED: HTTP Link Closed")))
//    {
//        memset(at_response_parser.buffer, 0, strlen((const char *)at_response_parser.buffer));
//    }
    
    return;
}

 /*******************************************************************************
 ** \brief  模块初始化流程
 **
 ** \param 
 **
 ** \retval Error 初始化失败
 **\retval OK 初始化完成
 ******************************************************************************/
en_result_t AtInit(AtInitStateMachineEnum at_init_state_machine)//发送命令并且核对返回值
{
    en_result_t send_statu = NoStatu;
    uint8_t *temporary_data;
    uint8_t network_registration_count = 0;
	uint8_t at_send_count = 0;//AT发送次数
    uint8_t cpin_error_count = 0;//cpin发送次数
    uint8_t enquiry_signal_intensity_count = 0;//查询信号强度次数
    //uint8_t dx;
    uint8_t i = 0;
    uint8_t signal_intensity;
    at_response_parser.at_init_state_machine = at_init_state_machine;
    send_flag = TRUE;
    send_waittime = 0;
    memset(at_response_parser.buffer, 0, at_response_parser.length);
    at_response_parser.length = 0;
    at_recive_succese = FALSE;
    at_recive_flag = FALSE;
    while(1)
    {
        SosKeyMonitor();
        if(mcu_status == MCU_KEY_OFF)
        {
            delay1ms(3000);
            //printf("I'm fine and you? ? ? ? ?\r\n");
            NbIotPowerOn();
            OpenGps();
            mcu_status = MCU_KEY_ON;
            at_response_parser.at_init_state_machine  = SM_AT_SENDING;
            //while(AtInit(SM_AT_SENDING) == Error);
        }
        if(at_recive_succese == TRUE)
        {
            if (at_response_parser.at_init_state_machine == SM_AT_SENDING)//查询AT
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.at_init_state_machine = SM_ATE0_SENDING;
                }
            }
        
            else if (at_response_parser.at_init_state_machine == SM_ATE0_SENDING)//查询取回显
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.at_init_state_machine = SM_CPIN_SENDING;
                }
            }
            
            else if (at_response_parser.at_init_state_machine == SM_CPIN_SENDING)	//查询SIM卡状态
            {
                if (strstr((const char *)at_response_parser.buffer, "+CPIN: READY") != NULL)
                {
                    //at_response_parser.at_init_state_machine = SM_CSQ_SENDING;
                    CloseRedBLN();
                    at_response_parser.at_init_state_machine = SM_CGSN_SENDING;
                    at_response_parser.gsm_state_machine = GSM_IDLE;
                    no_sim_time = 0;
                }
                else if(strstr((const char *)at_response_parser.buffer, "ERROR") != NULL)
                {
                    OpenRedBLN();
                    at_response_parser.gsm_state_machine = GSM_NOSIM;
                    if(cpin_error_count >= 10)// 没有sim卡 No sim Card
                    {
                        //printf("No Sim card\r\n");
                        NbIotReset();
                        at_response_parser.at_init_state_machine = SM_AT_SENDING;
                        send_statu = Error;
                    }
                    cpin_error_count++;
                }
                else
                {
                    send_statu = Error;
                }
                    
            }
            else if(at_response_parser.at_init_state_machine == SM_CGSN_SENDING)//查询IMEI
            {
                temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "+CGSN:");
                if(temporary_data != NULL)
                {
                    for(i=0; i<IMEI_LEN - 1; i++)
                    {
                        imei_data[i] = temporary_data[i+7];
                    }
                    imei_data[i] = '\0';
                    at_response_parser.at_init_state_machine = SM_CSQ_SENDING;
                }
            }                
            else if (at_response_parser.at_init_state_machine == SM_CSQ_SENDING)//查询信号强度
            {
                temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "+CSQ:");
                signal_intensity = Nmea_Str2num(temporary_data+6, 0);
                if(signal_intensity >= 12 && signal_intensity != 99)//12为建议信号强度
                {                    
                    at_response_parser.at_init_state_machine = SM_CREG_SENDING;
                }
                else //if (strstr((const char *)at_response_parser.buffer, "+CSQ: 99,99") == NULL)
                {
                    if(enquiry_signal_intensity_count >= QUERY_SIGNAL_INTENSITY_MAX)//无信号
                    {
                        NbIotReset();
                        at_response_parser.at_init_state_machine = SM_AT_SENDING;
                        send_statu = Error;
                    }
                    enquiry_signal_intensity_count++;
                }
            }         
            else if (at_response_parser.at_init_state_machine == SM_CREG_SENDING)//查询网络注册情况
            {
                //printf("%s", at_response_parser.buffer);
                temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "+CREG:");
                if(temporary_data[9] == '1' || temporary_data[9] == '5')
                {
                    if((mcu_status == MCU_ON) || (mcu_status == MCU_KEY_ON))
                    {
                        for(i=0; i<2; i++)
                        {
                            OpenBuzzer();
                            delay1ms(500);
                            CloseBuzzer();
                            delay1ms(500);
                        }
                        mcu_status = MCU_RUNING;
                    }
                    CloseBlueLed();
                    at_response_parser.at_init_state_machine = SM_CREG_SEND_OK;
                    network_registration_count = 0;
                    send_statu = Ok;
                }
                else
                {
                    //printf("what ??????\r\n");
                    OpenBlueLed();
                }
            }              
            memset(at_response_parser.buffer, 0, at_response_parser.length);
            at_response_parser.length = 0;
            at_recive_succese = FALSE;
            at_recive_flag = FALSE;
            if(send_statu != NoStatu)
            {
                send_flag = TRUE;
                return send_statu;
            }
            send_waittime = 1000;
            send_flag = FALSE;
        }
        else if(send_flag == TRUE)//1S内没收到回复重发
        {
            if(at_response_parser.at_init_state_machine == SM_AT_SENDING)
            {
                if(at_send_count >= AT_SEND_MAX)//串口可能出现问题
                {
                    NbIotReset();
                    return Error;
                }
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT\r");
                at_send_count++;
            }
            else if(at_response_parser.at_init_state_machine == SM_ATE0_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"ATE0\r");
            }
            else if(at_response_parser.at_init_state_machine == SM_CPIN_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CPIN?\r");
            }
            else if(at_response_parser.at_init_state_machine == SM_CGSN_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CGSN\r");
            }
            else if(at_response_parser.at_init_state_machine == SM_CSQ_SENDING)	
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CSQ\r");
            }
            else if(at_response_parser.at_init_state_machine == SM_CREG_SENDING)
            {
                if(network_registration_count >= NET_REG_COUNT_MAX)//注册网络失败
                {
                    //printf("network registration error\r\n")
                    NbIotReset();
                    at_response_parser.at_init_state_machine = SM_AT_SENDING;
                    return Error;
                }
                //printf("*******\r\n");
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CREG?\r");
                
                network_registration_count++;
            }
            else if(at_response_parser.at_init_state_machine == SM_CGSN_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CGSN\r");
            }
            send_flag = FALSE;
           
            if((gprs_send_parser.overtime == 0)||
               (gprs_send_parser.overtime_flag == FALSE))                
                    gprs_send_parser.overtime = RECIVE_OVERTIMER;
            Uart_EnableIrq(UARTCH0,UartRxIrq);
        }
        else if(gprs_send_parser.overtime_flag == TRUE)//超时
        {
            gprs_send_parser.overtime_flag = FALSE;
            send_flag = TRUE;
        }

    }
}
void GprsPPPSendInfo(void)
{
    int8_t set_ppp_count = 0;
    int8_t quiry_ppp_count = 0;
    en_result_t send_statu = NoStatu;
    AtInitStateMachineEnum at_init_state_machine;
    send_flag = TRUE;
    send_waittime = 0;
    while(1)
    {
        if (at_recive_succese == TRUE)
        {
            
            if(at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_SENDING)
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
                    set_ppp_count = 0;
                }
                else
                {
                    if(set_ppp_count >= SET_PPP_MAX)
                    {
                        at_init_state_machine = SM_AT_SENDING;
                        NbIotReset();
                        set_ppp_count = 0;
                    }
                    set_ppp_count ++;
                    send_statu = Error;
                }
            }
            else if(at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_Q_SENDING)
            {
                if (strstr((const char *)at_response_parser.buffer, "+XIIC:    1") != NULL)
                {
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SEND_OK;
                    send_statu = Ok;
                }
                else
                {
                    quiry_ppp_count = QUERY_SET_PPP_MAX;//test
                    if(quiry_ppp_count >= QUERY_SET_PPP_MAX)
                    {
                        at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_SENDING;
                        at_init_state_machine = SM_CPIN_SENDING;//回去查询SIM卡以及信号强度注册情况
                        if(set_ppp_count >= SET_PPP_MAX)
                        {
                            at_init_state_machine = SM_AT_SENDING;
                            NbIotReset();
                            set_ppp_count = 0;
                        }
                        set_ppp_count ++;
                        quiry_ppp_count = -1;
                        send_statu = Error;
                    }
                    quiry_ppp_count++;
                }
            }
            memset(at_response_parser.buffer, 0,at_response_parser.length);
            at_response_parser.length = 0;
            at_recive_succese = FALSE;
            at_recive_flag = FALSE;
            if(send_statu == Error)
            {
                while(AtInit(at_init_state_machine) == Error);
                send_statu = NoStatu;
            }
            else if(send_statu == Ok)
            {
                send_statu = NoStatu;
                send_flag = TRUE;
                //printf("at_response_parser.buffer = %s", at_response_parser.buffer);
                return ;
            }
            send_flag = FALSE;
            send_waittime = 1000;
        }
        else if(send_flag == TRUE)
        {
            if(at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+XIIC=1\r");
            }
            else if(at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_Q_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+XIIC?\r");
            }
            send_flag = FALSE;
            if((gprs_send_parser.overtime == 0)||
               (gprs_send_parser.overtime_flag == FALSE))                
            {            
                gprs_send_parser.overtime = RECIVE_OVERTIMER;
                gprs_send_parser.overtime_flag = FALSE;
            }
            Uart_EnableIrq(UARTCH0,UartRxIrq);
        }
        else if(gprs_send_parser.overtime_flag == TRUE)//超时
        {
            gprs_send_parser.overtime_flag = FALSE;
            at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
            NbIotReset();
            while(AtInit(at_init_state_machine) == Error);
            send_flag = TRUE;
        }
     
    }
}


void GprsHttpSendInfo(void)
{
    int8_t set_http_connect_count = 0;
    en_result_t send_statu = NoStatu;
    
    if((flash_memory_tidusrpwd_flag == 1)&&(flash_memory_id_flag == 1)&&(creat_system_id_flag == 1)) return;
    while(1)
    {   
        if((at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_Q_SENDING)||
           (at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_SENDING)) GprsPPPSendInfo();
        if (at_recive_succese == TRUE)
        {
            if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSPARA_URL_SENDING)//设置域名或IP
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPSPARA_PORT_SENDING;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    send_waittime = 1000;
                    send_flag = FALSE;
                }
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSPARA_PORT_SENDING)//设置端口
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPSETUP_SENDING;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    send_waittime = 1000;
                    send_flag = FALSE;
                };
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSETUP_SENDING)//建立连接
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPACTION_SENDING;
                    set_http_connect_count = 0;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    send_waittime = 1000;
                    send_flag = FALSE;
                }
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPACTION_SENDING)//执行HTTP请求
            {
                if (strstr((const char *)at_response_parser.buffer, ">") != NULL)
                {
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPSENDDATA_SENDING;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    send_waittime = 1000;
                    send_flag = FALSE;
                }
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSENDDATA_SENDING)//数据发送完毕
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_http_state = SM_GPRS_DEAL_REV_DATA;
                    Uart_EnableIrq(UARTCH0,UartRxIrq);
                }
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_DEAL_REV_DATA)
            {
                if (strstr((const char *)at_response_parser.buffer, "+HTTPRECV") != NULL)
                {
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;//XIIC建立一次就行，下一次查找不到再建立XIIC
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPSPARA_URL_SENDING;
                    ReciveDataDeal();
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    send_waittime = 1000;
                    send_flag = FALSE;
                }
            }
            else if(strstr((const char *)at_response_parser.buffer, "+HTTPACTION: OPERATION EXPIRED") != NULL)
            {
                send_statu = Error;
            }
            
            memset(at_response_parser.buffer, 0, at_response_parser.length);
            at_response_parser.length = 0;
            at_recive_succese = FALSE;
            at_recive_flag = FALSE;
            if((gprs_send_data_state == GPRS_CREATEXTERNID_OK)||( send_statu == Error))
            {
                send_flag = TRUE;
                send_waittime = 0;
                return ;
            }
        }
        else if(send_flag == TRUE)
        {

            if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSPARA_URL_SENDING)//设置域名或IP
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPPARA=url,\"pjjh.lnltiot.com/\"\r");
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSPARA_PORT_SENDING)//设置端口
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPPARA=port,80\r");
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSETUP_SENDING)//建立连接
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPSETUP\r");
                if(set_http_connect_count >= 40)
                {
                    NbIotReset();
                    while(AtInit(SM_AT_SENDING) == Error);
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_SENDING;
                    at_response_parser.gprs_http_state = SM_GPRS_HTTPSPARA_URL_SENDING;
                    set_http_connect_count = -1;
                }
                set_http_connect_count ++;
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPACTION_SENDING)//执行HTTP请求
            {
                HttpSendData();
            }
            else if(at_response_parser.gprs_http_state == SM_GPRS_HTTPSENDDATA_SENDING)//数据发送
            {
               Send_Command_Data(UARTCH0, (const uint8_t *)gprs_send_parser.buffer);
            }
            send_flag = FALSE;
            if((gprs_send_parser.overtime == 0)||
               (gprs_send_parser.overtime_flag == FALSE))
            {            
                    gprs_send_parser.overtime = RECIVE_OVERTIMER;
                    gprs_send_parser.overtime_flag = FALSE;
            }
            Uart_EnableIrq(UARTCH0,UartRxIrq);
        }
        else if(gprs_send_parser.overtime_flag == TRUE)
        {
            gprs_send_parser.overtime_flag = FALSE;
            at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
            at_response_parser.gprs_http_state = SM_GPRS_HTTPSPARA_URL_SENDING;
            send_flag = TRUE;
        }
    }
}
void MqttSendMessage(void)
{
    if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTCONNPARAM_SENDING)
    {
        sprintf((char *)gprs_send_parser.buffer, "AT+MQTTCONNPARAM=\"%s\",\"%s/%s\",\"%s\"\r" ,imei_data, tenantid, username, password);
    }
    if (at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTPUB_SENDING)
    {
        if(sos_key == 1)
        {
            gprs_send_data_state = GPRS_SENDSOS_ACTION;
            OpenRedBLN();
            sos_state = TRUE;
        }
        else
        {
            gprs_send_data_state = GPRS_SENDGPSINFO_ACTION;
            //sos_state = TRUE;
        }
        GetSendInfo(gprs_send_data_state);
    }
    Send_Command_Data(UARTCH0, (const uint8_t *)gprs_send_parser.buffer);
    gps_current_info.valid = FALSE;
    memset(gprs_send_parser.buffer, 0, gprs_send_parser.length);
    gprs_send_parser.length = 0;
    return ;
}


void MqttSendInfo(void)
{
    en_result_t send_statu = NoStatu;
    while(1)
    {
        if((at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_Q_SENDING)||
           (at_response_parser.gprs_ppp_state == SM_GPRS_XIIC_SENDING)) GprsPPPSendInfo();
        if (at_recive_succese == TRUE)
        {
            if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTCONNPARAM_SENDING)//设置域名或IP
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTCONN_SENDING;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
                    send_waittime = 500;
                    send_flag = FALSE;
                    NbIotReset();
                    while(AtInit(SM_AT_SENDING) == Error);
                }
                
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTCONN_SENDING)//建立连接
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTPUB_SENDING;
                    send_waittime = 0;
                    send_flag = TRUE;
                }
                else
                {
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
                    send_waittime = 500;
                    send_flag = FALSE;
                    NbIotReset();
                    while(AtInit(SM_AT_SENDING) == Error);
                }
                
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTPUB_SENDING)//发布数据
            {
                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
                {
                    send_waittime = 0;
                    send_flag = TRUE;
//                    CloseGreenLed();
//                    CloseBlueLed();
                    if(gprs_send_data_state == GPRS_SENDSOS_ACTION)
                    {
                        //CloseRedLed();
                        CloseRedBLN();
                        sos_key = FALSE;
                        EnableNvic(PORTD_IRQn, IrqLevel3, TRUE);
                    }
                    if((sos_key == FALSE)&&(IsEmpty(&gps_queue) == FALSE)&&(sos_state == FALSE))
                    {
                        CloseNbIot();
                        CloseGps();
                        //OpenRedLed();
                        EnableNvic(RTC_IRQn,IrqLevel3,TRUE);
                        Lpm_GotoLpmMode();
                        //CloseRedLed();
                        delay1ms(3000);
                        NbIotPowerOn();
                        OpenGps();
                        while(AtInit(SM_AT_SENDING) == Error);
                        EnableNvic(RTC_IRQn,IrqLevel3,FALSE);
                        Rtc_ClrIrqStatus(RtcPrdf);
                    }
                }
                else
                {
//                    OpenBlueLed();
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
                    send_waittime = 500;
                    send_flag = FALSE;
                }
                at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTSTATE_SENDING;
                //at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTCONNPARAM_SENDING;
                
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTSTATE_SENDING)
            {
                if (strstr((const char *)at_response_parser.buffer, "+MQTTSTATE:1") != NULL)
                {
                    at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTPUB_SENDING;
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
                }
                else if (strstr((const char *)at_response_parser.buffer, "+MQTTSTATE:0") != NULL)
                {
                    OpenBlueLed();
                    at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_SENDING;
                    at_response_parser.gprs_mqtt_statu = SM_GPRS_MQTTCONNPARAM_SENDING;
                    //at_response_parser.at_init_state_machine = SM_AT_SENDING;
                    send_statu = Error;
                }
                send_flag = TRUE;
                send_waittime = 0;
            }
            memset(at_response_parser.buffer, 0, at_response_parser.length);
            at_response_parser.length = 0;
            at_recive_succese = FALSE;
            at_recive_flag = FALSE;
            if(gprs_send_data_state == GPRS_CREATAGENT_OK)
            {
                send_flag = TRUE;
                send_waittime = 0;
                return ;
            }
            if(send_statu == Error)
            {
                send_statu = NoStatu;
                NbIotReset();
                while(AtInit(SM_AT_SENDING) == Error);
            }
        }
        else if(send_flag == TRUE)
        {

            if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTCONNPARAM_SENDING)//设置域名或IP
            {
                MqttSendMessage();
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTCONN_SENDING)//建立连接
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+MQTTCONN=\"pjjh.lnltiot.com:1883\",0,60\r");
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTPUB_SENDING)//发布消息
            {
                MqttSendMessage();
            }
            else if(at_response_parser.gprs_mqtt_statu == SM_GPRS_MQTTSTATE_SENDING)
            {
                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+MQTTSTATE?\r");
            }
            send_flag = FALSE;
            if((gprs_send_parser.overtime == 0)||
               (gprs_send_parser.overtime_flag == FALSE))                
            {            
                gprs_send_parser.overtime = RECIVE_OVERTIMER;
                gprs_send_parser.overtime_flag = FALSE;
            }
            Uart_EnableIrq(UARTCH0,UartRxIrq);
        }
        else if(gprs_send_parser.overtime_flag == TRUE)
        {
            gprs_send_parser.overtime_flag = FALSE;
            at_response_parser.gprs_ppp_state = SM_GPRS_XIIC_Q_SENDING;
            send_flag = TRUE;
        }
    }
}
