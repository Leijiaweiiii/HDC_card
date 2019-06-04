#ifndef __ARRY__QUEUE__H__
#define __ARRY__QUEUE__H__
#include "gps.h"

#define QUEUE_SIZE 5  //能存储最多的GPS数据个数

typedef struct Queue
{
    GPS_INFO gps_info[QUEUE_SIZE];
    int8_t head;
    int8_t tail;
    int8_t size;
    
}Gps_Queue;
void EnQueue(GPS_INFO gps_info, Gps_Queue *gps_queue);

//出队列
GPS_INFO DeQueue(Gps_Queue *gps_queue);

//判断队列是否为空
uint8_t IsEmpty(Gps_Queue *gps_queue);

uint8_t IsFull(Gps_Queue *gps_queue);

extern Gps_Queue gps_queue;
#endif

