//#include "mqtt_send_message.h"

//#define SET_PPP_MAX                3    //建ppp连接失败最高次数
//#define QUERY_SET_PPP_MAX          20   //查询ip地址最高次数
//#define QUERY_SIGNAL_INTENSITY_MAX 40   //查询信号强度最高次数
//#define QUERY_SIM_CARD_MAX         10   //查询SIM卡状态最高次数


//AtResponseDistill at_response_parser = 
//                                       {GSM_IDLE,
//                                        SM_AT_SENDING, 
//                                        SM_GPRS_XIIC_SENDING,
//                                         {0}, 
//                                         0, 
//                                         0,
//                                       };
//uint8_t imei_data[IMEI_LEN]="869060030502128";

//static uint8_t username_password[45] = {"pjjh/"};
//uint8_t system_id[6] = {"36711"};
//static char base64_char[60] = {"cGpqaC9kZXZpY2VfODY5MDYwMDMwNTAyMTI4OjhKTEIoazd2ckk="};
//char *usr_pwd_base64 = base64_char;
//static const char *alpha_base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//static uint8_t send_body[1024];
//uint8_t creat_system_id_flag = 0;

//uint8_t flash_memory_id_flag = 1;
//uint8_t flash_memory_tidusrpwd_flag = 1;
//uint8_t flash_memory_id_len = 0;
//uint8_t flash_memory_tidusrpwd_flag = 0;
//uint8_t gps_info_valid = 0;
//uint8_t gps_info_send_statu = 0;
//uint8_t sim_card_flag = 0;




// /*******************************************************************************
// ** \brief  将ASCII码转换位Base64编码
// **
// ** \param [in] buf: ASCII字符串
// **        size: ASCII字符串长度
// **        base64Char: 返回的Base64编码字符串
// **        
// **
// ** \retval 字符串 
// ******************************************************************************/
//static char *Encode(const char *buf, const int size, char *base64Char) 
//{
//    int a = 0;
//    int i = 0;
//    while (i < size) {
//        char b0 = buf[i++];
//        char b1 = (i < size) ? buf[i++] : 0;
//        char b2 = (i < size) ? buf[i++] : 0;
//         
//        int int63 = 0x3F; //  00111111
//        int int255 = 0xFF; // 11111111
//        base64Char[a++] = alpha_base[(b0 >> 2) & int63];
//        base64Char[a++] = alpha_base[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
//        base64Char[a++] = alpha_base[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
//        base64Char[a++] = alpha_base[b2 & int63];
//    }
//    switch (size % 3) {
//        case 1:
//            base64Char[--a] = '=';
//        case 2:
//            base64Char[--a] = '=';
//    }
//    return base64Char;
//}


///*******************************************************************************
//** \brief  获取用户名以及密码将用户名密码存入flash
// **
// ** \param [in] buf: ASCII字符串
// **        size: ASCII字符串长度
// **        base64Char: 返回的Base64编码字符串
// **        
// **
// ** \retval 字符串 
// ******************************************************************************/
//en_result_t GetUsrpwdToFlash(void)
//{
//}

////static en_result_t find_str(uint8_t des_str[])
////{
////    if (strstr((const char *)at_response_parser.buffer, (const char *)des_str) != NULL)
////    {
////        return Ok;
////    }
////    else
////    {
////        return Error;
////    }
////}

// /*******************************************************************************
//** \brief  需要上传HTTP的数据
// **
// ** \param [in] u8Idx通道号
// **        data_buf 串口接收到的数据
// **
// ** \retval Error 获取成功
// **\retval OK 获取失败
// *******************************************************************************/
//uint8_t *GetSendInfo(GprsSendDataStatu gprs_send_data_state)
//{
//    char time[30] = {"2019-04-19T18:35:14.000+08:00"};
//    /************************1.注册设备*************************
//    ***********************************************************/
//    if(gprs_send_data_state == GPRS_HTTPS_REGISTER_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer, 
//        "POST /devicecontrol/deviceCredentials HTTP/1.1\r\n"
//        "HOST: pjjh.lnltiot.com\r\n"
//        "Content-Type: application/json\r\n"
//        "Accept: application/vnd.com.nsn.cumulocity.newDeviceRequest+json;\r\n"
//        "Authorization: Basic ZGV2aWNlOmRldmljZUAxMjM=\r\n"  
//        "connection: close\r\n"
//        "Content-Length: 24\r\n\r\n{\"id\":\"%s\"}",imei_data);
//    }
//    /***********************2.创建设备凭证**********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_CREDENTIALS_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer, 
//        "POST /devicecontrol/deviceCredentials HTTP/1.1\r\n"                          
//        "HOST: pjjh.lnltiot.com\r\n"                     
//        "Content-Type: application/json\r\n"
//        "Authorization: Basic bWFuYWdlbWVudC9kZXZpY2Vib290c3RyYXA6RmhkdDFiYjFm\r\n"
//        "connection: close\r\n"
//        "Content-Length: 24\r\n\r\n"
//        "{\"id\":\"%s\"}" , imei_data);
//    }
//    /*********************3.创建设备凭证***********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_UPDATA_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer, 
//        "PUT /devicecontrol/newDeviceRequests/%s HTTP/1.1\r\n"
//        "HOST: pjjh.lnltiot.com\r\n"
//        "Content-Type: application/json\r\n"
//        "Authorization: Basic ZGV2aWNlOmRldmljZUAxMjM=\r\n"
//        "connection: close\r\n"
//        "Content-Length: 22\r\n\r\n"
//        "{\"status\": \"ACCEPTED\"}", imei_data);
//    }
//    /**********************4.获取用户名密码********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_GETUSERINFO_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer,
//        "POST /devicecontrol/deviceCredentials HTTP/1.1"
//        "\r\nHOST: pjjh.lnltiot.com"
//        "\r\nContent-Type: application/json"
//        "\r\nAuthorization: Basic bWFuYWdlbWVudC9kZXZpY2Vib290c3RyYXA6RmhkdDFiYjFm"
//        "\r\nconnection: close"
//        "\r\nContent-Length: 24\r\n\r\n"
//        "{\"id\":\"%s\"}", imei_data);
//    }
//    /************************5.创建Agent************************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_CREATAGENT_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer,
//        "POST /inventory/managedObjects HTTP/1.1\r\n"
//        "HOST: pjjh.lnltiot.com\r\n"
//        "Content-Type: application/json\r\n"
//        "Accept: application/json\r\n"
//        "Authorization: Basic %s\r\n"
//        "connection: close\r\n"
//        "Content-Length: 76\r\n\r\n"
//        "{\"name:\"\"%s\",\"com_cumulocity_model_Agent\":{},\"c8y_IsDevice\":{}}" ,usr_pwd_base64, imei_data);
//    }
//    /************************6.创建外部id***********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_CREATEXTERNID_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer,
//        "POST /identity/globalIds/%s/externalIds HTTP/1.1\r\n"
//        "HOST: pjjh.lnltiot.com\r\n"
//        "Content-Type: application/vnd.com.nsn.cumulocity.externalId+json\r\n"
//        "Accept: application/vnd.com.nsn.cumulocity.externalId+json\r\n"
//        "Authorization: Basic %s\r\n"
//        "connection: close\r\n"
//        "Content-Length: 54\r\n\r\n"
//        "{\"externalId\":\"%s\",\"type\":\"c8y_WorkCard\"}" , system_id, usr_pwd_base64, imei_data);
//    }
//    /************************7.发送GPS信息********************
//    ***********************************************************/
//    else if(gprs_send_data_state == GPRS_HTTPS_SENDGPSINFO_ACTION)
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
//        sprintf((char *)http_send_info.buffer,
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
//    else if(gprs_send_data_state == GPRS_HTTPS_SENDSOS_ACTION)
//    {
//        sprintf((char *)http_send_info.buffer,
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
//    return http_send_info.buffer;
//}

//void HttpSendData(void)
//{
//    uint8_t http_action_data_buff[23] = {0};
//    uint8_t http_action_data[17] = "AT+HTTPACTION=99,";
//    if(flash_memory_id_flag == 0)
//    {
//        if(gprs_send_data_state == GPRS_HTTPS_REGISTER_ACTION)
//        {
//            GetSendInfo(gprs_send_data_state);
//            /*gprs_send_data_state = GPRS_HTTPS_REGISTER_ACTION;*///1.注册设备
//        }
//        else if(gprs_send_data_state == GPRS_HTTPS_REGISTER_OK)//2.创建设备凭证
//        {
//            gprs_send_data_state = GPRS_HTTPS_CREDENTIALS_ACTION;
//            GetSendInfo(gprs_send_data_state);
//        }
//        else if(gprs_send_data_state == GPRS_HTTPS_CREDENTIALS_OK)//3.创建设备凭证
//        {
//            GetSendInfo(gprs_send_data_state);
//            gprs_send_data_state = GPRS_HTTPS_UPDATA_ACTION;
//        }
//        else if(gprs_send_data_state == GPRS_HTTPS_UPDATA_OK)//4.获取用户名密码
//        {
//            GetSendInfo(gprs_send_data_state);
//            gprs_send_data_state = GPRS_HTTPS_GETUSERINFO_ACTION;
//        }
//    }
//    else if(flash_memory_id_flag == 1)
//    {
//        flash_memory_id_flag = 2;
//        gprs_send_data_state = GPRS_HTTPS_GETUSERINFO_OK;
//    }
//    if(flash_memory_tidusrpwd_flag == 0)
//    {
//        if(gprs_send_data_state == GPRS_HTTPS_GETUSERINFO_OK )//5.创建Agent
//        {
//            gprs_send_data_state = GPRS_HTTPS_CREATAGENT_ACTION;
//            GetSendInfo(gprs_send_data_state);
//        }
//    }
//    else if(flash_memory_tidusrpwd_flag == 1)
//    {
//        gprs_send_data_state = GPRS_HTTPS_CREATAGENT_OK;
//        flash_memory_tidusrpwd_flag = 2;
//        creat_system_id_flag = 1;
//    }
//    if(gprs_send_data_state == GPRS_HTTPS_SENDSOS_ACTION)//发送求救
//    {
//        GetSendInfo(gprs_send_data_state);
//        sos_key = 2;
//        gprs_send_data_state = GPRS_HTTPS_SENDSOS_OK;
//    }
//    else if(gprs_send_data_state == GPRS_HTTPS_CREATAGENT_OK)//6.创建外部id
//    {
//        gprs_send_data_state = GPRS_HTTPS_CREATEXTERNID_ACTION;
//        GetSendInfo(gprs_send_data_state);
//    }
//    else if((gprs_send_data_state == GPRS_HTTPS_CREATEXTERNID_OK)||
//            (gprs_send_data_state == GPRS_HTTPS_SENDGPSINFO_OK)||
//            (gprs_send_data_state == GPRS_HTTPS_DEALSOS_OK))//7.发送定位数据
//    {
//        gprs_send_data_state = GPRS_HTTPS_SENDGPSINFO_ACTION;
//        GetSendInfo(gprs_send_data_state);
//    }
//    sprintf((char *)http_action_data_buff, "%s%d\r", http_action_data, strlen((const char *)http_send_info.buffer));
//    Send_Command_Data(UARTCH0, (const uint8_t *)http_action_data_buff);
//    return ;
//}     



//void ReciveDataDeal(void)
//{
//    if (gprs_send_data_state == GPRS_HTTPS_REGISTER_ACTION)
//    {
//        if ((strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)||
//            (strstr((const char *)at_response_parser.buffer, "already exists") != NULL))
//        {
//            gprs_send_data_state = GPRS_HTTPS_REGISTER_OK;
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_CREDENTIALS_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "PENDING_ACCEPTANCE") != NULL)
//        {
//            gprs_send_data_state = GPRS_HTTPS_CREDENTIALS_OK;
//            //printf("%s\n", at_response_parser.buffer);
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_UPDATA_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "200 OK") != NULL)
//        {
//            gprs_send_data_state = GPRS_HTTPS_UPDATA_OK;
//            //printf("%s\n", at_response_parser.buffer);
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_GETUSERINFO_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
//        {
//            gprs_send_data_state = GPRS_HTTPS_GETUSERINFO_OK;
//            usr_pwd_base64 = Encode((char *)username_password, strlen((const char *)username_password), (char *)base64_char);
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_CREATAGENT_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
//        {
//           
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_CREATEXTERNID_ACTION)
//    {
//        if ((strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)||
//            (strstr((const char *)at_response_parser.buffer, "already exists") != NULL))
//        {
//            gprs_send_data_state = GPRS_HTTPS_CREATEXTERNID_OK;
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_SENDSOS_OK)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "ACTIVE") != NULL)
//        {
//            gprs_send_data_state = GPRS_HTTPS_DEALSOS_OK;
//            sos_key = 0;
//        }
//    }
//    else if (gprs_send_data_state == GPRS_HTTPS_SENDGPSINFO_ACTION)
//    {
//        if (strstr((const char *)at_response_parser.buffer, "201 Created") != NULL)
//        {
//            gprs_send_data_state = GPRS_HTTPS_SENDGPSINFO_OK;
//        }
//        if ((strstr((const char *)at_response_parser.buffer, "500 Server Error")))
//        {
//            gprs_send_data_state = GPRS_HTTPS_SENDGPSINFO_OK;
//        }
//    }
//    if ((strstr((const char *)at_response_parser.buffer, "ERROR"))||
//        (strstr((const char *)at_response_parser.buffer, "+HTTPACTION: OPERATION EXPIRED"))||
//        (strstr((const char *)at_response_parser.buffer, "+HTTPCLOSED: HTTP Link Closed")))
//    {
//        memset(at_response_parser.buffer, 0, strlen((const char *)at_response_parser.buffer));
//    }
//    return;
//}

//static en_result_t NB_IOT_reset(void)//NB_IOT重启
//{
//    return Ok;
//}

// /*******************************************************************************
// ** \brief  模块初始化流程
// **
// ** \param 
// **
// ** \retval Error 初始化失败
// **\retval OK 初始化完成
// ******************************************************************************/
//en_result_t AtInit(void)//发送命令并且核对返回值
//{
//    en_result_t send_statu = NoStatu;
//    uint8_t *temporary_data;
//    uint8_t network_registration_count = 0;
//	uint8_t at_send_count = 0;//AT发送次数
//    uint8_t cpin_error_count = 0;//cpin发送次数
//    uint8_t enquiry_signal_intensity_count = 0;//查询信号强度次数
//    //uint8_t dx;
//    uint8_t signal_intensity;
//    while(1)
//    {
//        if(at_recive_succese == 1)
//        {
//            if (at_response_parser.at_init_state_machine == SM_AT_SENDING)//查询AT
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    at_response_parser.at_init_state_machine = SM_ATE0_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//        
//            else if (at_response_parser.at_init_state_machine == SM_ATE0_SENDING)//查询取回显
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    at_response_parser.at_init_state_machine = SM_CPIN_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            
//            else if (at_response_parser.at_init_state_machine == SM_CPIN_SENDING)	//查询SIM卡状态
//            {
//                if (strstr((const char *)at_response_parser.buffer, "+CPIN: READY") != NULL)
//                {
//                    at_response_parser.at_init_state_machine = SM_CSQ_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//                else if(strstr((const char *)at_response_parser.buffer, "+CPIN: ERROR") != NULL)
//                {
//                    if(cpin_error_count >= 10)// 没有sim卡 No sim Card
//                    {
//                        //printf("No Sim card\r\n");
//                        NbIotReset();
//                        at_response_parser.at_init_state_machine = SM_AT_SENDING;
//                        send_statu = Error;
//                    }
//                    cpin_error_count++;
//                }
//                else
//                {
//                    send_statu = Error;
//                }
//                    
//            }                    
//            else if (at_response_parser.at_init_state_machine == SM_CSQ_SENDING)//查询信号强度
//            {
//                temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "+CSQ:");
//                signal_intensity = Nmea_Str2num(temporary_data+6, 0);
//                if(signal_intensity >= 12 && signal_intensity != 99)//12为建议信号强度
//                {                    
//                    at_response_parser.at_init_state_machine = SM_CREG_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//                else //if (strstr((const char *)at_response_parser.buffer, "+CSQ: 99,99") == NULL)
//                {
//                    if(enquiry_signal_intensity_count >= QUERY_SIGNAL_INTENSITY_MAX)//无信号
//                    {
//                        NbIotReset();
//                        at_response_parser.at_init_state_machine = SM_AT_SENDING;
//                        //printf("enquiry_signal_intensity_error\r\n");
//                        send_statu = Error;
//                    }
//                    enquiry_signal_intensity_count++;
//                }
//            }
//                        
//            else if (at_response_parser.at_init_state_machine == SM_CREG_SENDING)//查询网络注册情况
//            {
//                temporary_data = (uint8_t*)strstr((const char *)at_response_parser.buffer, "+CREG:");
//                //printf("%s\n", temporary_data);
//                if(temporary_data[9] == '1' || temporary_data[9] == '5')
//                {
//                    at_response_parser.at_init_state_machine = SM_CREG_SEND_OK;
//                    network_registration_count = 0;
//                    send_flag = 1;
//                    send_waittime = 0;
//                    send_statu = Ok;
//                }
//            }              
//            memset(at_response_parser.buffer, 0, strlen((const char *)at_response_parser.buffer));
//            at_response_parser.length = 0;
//            at_recive_succese = 0;
//            at_recive_flag = 0;
//            if(send_statu != NoStatu)
//            {
//                return send_statu;
//            }
//        }
//        else if(send_flag)//1S内没收到回复重发
//        {
//            if(at_response_parser.at_init_state_machine == SM_AT_SENDING)
//            {
//                if(at_send_count >= 10)//串口可能出现问题
//                {
//                    //printf("send at error\r\n");
//                    NbIotReset();
//                    return Error;
//                }
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT\r");
//                at_send_count++;
//            }
//            else if(at_response_parser.at_init_state_machine == SM_ATE0_SENDING)
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"ATE0\r");
//            }
//            else if(at_response_parser.at_init_state_machine == SM_CPIN_SENDING)
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CPIN?\r");
//            }
//            else if(at_response_parser.at_init_state_machine == SM_CSQ_SENDING)	
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CSQ\r");
//            }
//            else if(at_response_parser.at_init_state_machine == SM_CREG_SENDING)
//            {
//                if(network_registration_count >= 40)//注册网络失败
//                {
//                    //printf("network registration error\r\n")
//                    NbIotReset();
//                    at_response_parser.at_init_state_machine = SM_AT_SENDING;
//                    return Error;
//                }
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CREG?\r");
//                network_registration_count++;
//            }
//            else if(at_response_parser.at_init_state_machine == SM_CGSN_SENDING)
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+CGSN\r");
//            }
//            send_waittime = 1000;
//            send_flag = 0;
//            Uart_EnableIrq(UARTCH0,UartRxIrq);
//        }
//    }
//}



//void GprsHttpSendInfo(void)
//{
//    char set_ppp_count = 0;
//    char quiry_ppp_count = 0;
//    char set_http_connect_count = 0;
//    en_result_t send_statu = NoStatu;
//    while(1)
//    {
//        if (at_recive_succese == 1)
//        {
//            if(gprs_http_state == SM_GPRS_XIIC_SENDING)
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_XIIC_Q_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_XIIC_Q_SENDING)
//            {
//                if (strstr((const char *)at_response_parser.buffer, "+XIIC:    1") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_HTTPSPARA_URL_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//                else
//                {
//                    quiry_ppp_count = QUERY_SET_PPP_MAX;//test
//                    if(quiry_ppp_count >= QUERY_SET_PPP_MAX)
//                    {
//                        gprs_http_state = SM_GPRS_XIIC_SENDING;
//                        at_init_state_machine = SM_CPIN_SENDING;//回去查询SIM卡以及信号强度注册情况
//                        if(set_ppp_count >= SET_PPP_MAX)
//                        {
//                            at_init_state_machine = SM_AT_SENDING;
//                            NB_IOT_reset();
//                            set_ppp_count = 0;
//                        }
//                        quiry_ppp_count = -1;
//                        send_statu = Error;
//                    }
//                    quiry_ppp_count++;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSPARA_URL_SENDING)//设置域名或IP
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_HTTPSPARA_PORT_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSPARA_PORT_SENDING)//设置端口
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_HTTPSETUP_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSETUP_SENDING)//建立连接
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_HTTPACTION_SENDING;
//                    set_http_connect_count = 0;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPACTION_SENDING)//执行HTTP请求
//            {
//                if (strstr((const char *)at_response_parser.buffer, ">") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_HTTPSENDDATA_SENDING;
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSENDDATA_SENDING)//数据发送完毕
//            {
//                if (strstr((const char *)at_response_parser.buffer, "OK") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_DEAL_REV_DATA;
//                    send_flag = 1;
//                    send_waittime = 0;
//                    Uart_EnableIrq(UARTCH0,UartRxIrq);
//                }
//            }
//            else if(gprs_http_state == SM_GPRS_DEAL_REV_DATA)
//            {
//                if (strstr((const char *)at_response_parser.buffer, "+HTTPRECV") != NULL)
//                {
//                    gprs_http_state = SM_GPRS_XIIC_Q_SENDING;//XIIC建立一次就行，下一次查找不到再建立XIIC
//                    ReciveDataDeal();
//                    send_flag = 1;
//                    send_waittime = 0;
//                }
//            }
//            at_response_parser.length = 0;
//            at_recive_succese = 0;
//            at_recive_flag = 0;
//            memset(at_response_parser.buffer, 0, strlen((const char *)at_response_parser.buffer));
//            if(send_statu != NoStatu)
//            {
//                while(AtInit() == Error);
//                send_statu = NoStatu;
//            }
//        }
//        else if(send_flag == 1)
//        {
//            if(gprs_http_state == SM_GPRS_XIIC_SENDING)
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+XIIC=1\r");
//            }
//            else if(gprs_http_state == SM_GPRS_XIIC_Q_SENDING)
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+XIIC?\r");
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSPARA_URL_SENDING)//设置域名或IP
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPPARA=url,\"pjjh.lnltiot.com/\"\r");
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSPARA_PORT_SENDING)//设置端口
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPPARA=port,80\r");
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSETUP_SENDING)//建立连接
//            {
//                Send_Command_Data(UARTCH0, (const uint8_t *)"AT+HTTPSETUP\r");
//                if(set_http_connect_count >= 40)
//                {
//                    gprs_http_state = SM_GPRS_XIIC_SENDING;
//                    set_http_connect_count = -1;
//                }
//                set_http_connect_count ++;
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPACTION_SENDING)//执行HTTP请求
//            {
//                HttpSendData();
//            }
//            else if(gprs_http_state == SM_GPRS_HTTPSENDDATA_SENDING)//数据发送
//            {
//               Send_Command_Data(UARTCH0, (const uint8_t *)http_send_info.buffer);
//            }
//            send_flag = 0;
//            send_waittime = 1000;
//        }
//    }
//}

