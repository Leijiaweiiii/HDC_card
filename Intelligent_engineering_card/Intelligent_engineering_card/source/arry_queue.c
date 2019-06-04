#include "arry_queue.h"
#include <string.h>
#include <stdio.h>
//#define QUEUE_SIZE 5

//static int queue[QUEUE_SIZE];
//static int head = 0;
//static int tail = 0;
//GPS_INFO gps_current_info;
//������
Gps_Queue gps_queue = {{0},0,0,0};

void EnQueue(GPS_INFO gps_info, Gps_Queue *gps_queue)
{
    //Ҫ���ж϶����Ƿ�����
    if ((gps_queue->tail == QUEUE_SIZE - 1)||(gps_queue->size == QUEUE_SIZE))
    {
        //printf("��������������ͷ\n");
        gps_queue->head = (gps_queue->head + 1) % QUEUE_SIZE;
    }
    if (gps_queue->size == 0) 
    {
        gps_queue->head = 0;
        gps_queue->tail = 0;
    }
    //printf("gps_info ==============%f\r\n", gps_info.height);
    //memcpy(&gps_queue->gps_info[gps_queue->tail], &gps_info, sizeof(gps_info));
    gps_queue->gps_info[gps_queue->tail] = gps_info;
    //printf("gps_queue->gps_info[gps_queue->tail] ==============%f\r\n", gps_queue->gps_info[gps_queue->tail].height);
    if((++gps_queue->size) >= QUEUE_SIZE) gps_queue->size = QUEUE_SIZE;
    gps_queue->tail = (gps_queue->tail + 1) % QUEUE_SIZE;
    
}

//������
GPS_INFO DeQueue(Gps_Queue *gps_queue)
{
    GPS_INFO gps_info;
    
    
    if (gps_queue->size == 0)
    {
        //printf("����Ϊ�գ�Ԫ���޷�������\n");
    }
    else
    {
        gps_queue->tail--;
        if((gps_queue->tail) < 0)
        {
            if(gps_queue->tail < gps_queue->head)
            {
                gps_queue->tail = QUEUE_SIZE -1;
            }
//            else
//            {
//                gps_queue->tail = gps_queue->head;//�ȳ���������
//            }
            //gps_queue->head = (gps_queue->head + 1) % QUEUE_SIZE;
        }
        //printf("*************gps_queue->tail = %d******************\r\n", gps_queue->tail);
        gps_info = gps_queue->gps_info[gps_queue->tail];
        gps_queue->size --;

    }

    //printf("%d\n",temp);
    return gps_info;
}

//�ж϶����Ƿ�Ϊ��
uint8_t IsEmpty(Gps_Queue *gps_queue)
{
    
    if (gps_queue->size == 0) 
    {
        //printf("����Ϊ��\n");
        return 0;
    }
    //printf("���в�Ϊ��\n");
    return 1;
}

//�ж϶����Ƿ�����
/**
 *  �������ж϶����ĵķ�����
 ����һ����Ԫ�����ֶӿպͶ��������ʱ����һ�����е�Ԫ���������Ĵ�СΪSize,��ôʵ��ֻ�ܴ��(Size-1)��Ԫ�ء������ǱȽϳ��õ������ķ�ʽ��
 *
 */
uint8_t IsFull(Gps_Queue *gps_queue)
{

    if ((gps_queue->tail + 1) % QUEUE_SIZE == gps_queue->head)
    {
        //printf("��������\n");
        return 1;
    }

    //printf("����δ��\n");
    return 0;
}

//��ӡ������Ԫ��
void PrintQueue()
{

//    for (int i = head; i < tail; i++)d
//    {
//        printf("%d ",queue[i]);
//    }
//    printf("\n");
}
