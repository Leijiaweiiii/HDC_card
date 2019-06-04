#ifndef __ARRY__QUEUE__H__
#define __ARRY__QUEUE__H__
#include "gps.h"

#define QUEUE_SIZE 5  //�ܴ洢����GPS���ݸ���

typedef struct Queue
{
    GPS_INFO gps_info[QUEUE_SIZE];
    int8_t head;
    int8_t tail;
    int8_t size;
    
}Gps_Queue;
void EnQueue(GPS_INFO gps_info, Gps_Queue *gps_queue);

//������
GPS_INFO DeQueue(Gps_Queue *gps_queue);

//�ж϶����Ƿ�Ϊ��
uint8_t IsEmpty(Gps_Queue *gps_queue);

uint8_t IsFull(Gps_Queue *gps_queue);

extern Gps_Queue gps_queue;
#endif

