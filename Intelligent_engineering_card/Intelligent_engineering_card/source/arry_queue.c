#include "arry_queue.h"
#include <string.h>
#include <stdio.h>
//#define QUEUE_SIZE 5

//static int queue[QUEUE_SIZE];
//static int head = 0;
//static int tail = 0;
//GPS_INFO gps_current_info;
//进队列
Gps_Queue gps_queue = {{0},0,0,0};

void EnQueue(GPS_INFO gps_info, Gps_Queue *gps_queue)
{
    //要先判断队列是否已满
    if ((gps_queue->tail == QUEUE_SIZE - 1)||(gps_queue->size == QUEUE_SIZE))
    {
        //printf("队列已满，覆盖头\n");
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

//出队列
GPS_INFO DeQueue(Gps_Queue *gps_queue)
{
    GPS_INFO gps_info;
    
    
    if (gps_queue->size == 0)
    {
        //printf("队列为空，元素无法出队列\n");
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
//                gps_queue->tail = gps_queue->head;//先出最新数据
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

//判断队列是否为空
uint8_t IsEmpty(Gps_Queue *gps_queue)
{
    
    if (gps_queue->size == 0) 
    {
        //printf("队列为空\n");
        return 0;
    }
    //printf("队列不为空\n");
    return 1;
}

//判断队列是否已满
/**
 *  我这里判断队满的的方法：
 牺牲一个单元来区分队空和队满，入队时少用一个队列单元。如果数组的大小为Size,那么实际只能存放(Size-1)个元素。（这是比较常用的判满的方式）
 *
 */
uint8_t IsFull(Gps_Queue *gps_queue)
{

    if ((gps_queue->tail + 1) % QUEUE_SIZE == gps_queue->head)
    {
        //printf("队列已满\n");
        return 1;
    }

    //printf("队列未满\n");
    return 0;
}

//打印出队列元素
void PrintQueue()
{

//    for (int i = head; i < tail; i++)d
//    {
//        printf("%d ",queue[i]);
//    }
//    printf("\n");
}
