#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"`
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"


/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 List_t                  TestList;           
        
 */
// List_t                  TestList; 
// ListItem_t              ListItem1;          
// ListItem_t              ListItem2;         
// ListItem_t              ListItem3;  

#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */


 QueueHandle_t   key_queue;      /* 队列句柄 */
 QueueHandle_t   big_data;
 char buff[100]={"大数组drthdydutduryfuyt"};
QueueSetHandle_t queueset_handler; /* 队列集句柄 */
QueueHandle_t   queue1_handler; /* 队列1句柄 */
QueueHandle_t   semp_handler; /* 队列2句柄 */
 
/******************************************************************************************************/
    

 
void freertos_demo(void)
{
    key_queue=xQueueCreate(2, sizeof(uint8_t));
    if(key_queue!=NULL){
        printf("key_queue success！\r\n");
    }
    else{
        printf("key_queue创建失败！\r\n");
    }
     big_data=xQueueCreate(1, sizeof(char*));
    if(big_data!=NULL){
        printf("big_data sucess！\r\n");
    }
    else{
        printf("big_data创建失败！\r\n");
    }
    lcd_show_string(10, 10, 220, 32, 32, "FreeRTOS", RED);
    lcd_show_string(10, 47, 220, 24, 24, "LED Blink Demo", RED);
    lcd_show_string(10, 76, 220, 16, 16, "LED0: 500ms", BLUE);
    //lcd_show_string(10, 96, 220, 16, 16, "LED1: 200ms", BLUE);
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    queueset_handler= xQueueCreateSet(2); /* 创建一个包含2个成员的队列集 */
    semp_handler=xSemaphoreCreateBinary();
    
        if(queueset_handler!=NULL){
            printf("queueset_handler success!\r\n");
        }
        else{
            printf("queueset_handler fail!\r\n");
        }
        queue1_handler=xQueueCreate(1,sizeof(uint8_t));
        xQueueAddToSet(queue1_handler, queueset_handler); /* 将队列1添加到队列集中 */
        xQueueAddToSet(semp_handler, queueset_handler); /* 将信号量添加到队列集中 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
  

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task1(void *pvParameters)
{
    (void)pvParameters;
   
    uint8_t key=0;
    BaseType_t err=0;
    char*buf=buff;
    
    while(1)
    {
    
    key=key_scan(0);
    
    if(key==KEY0_PRES)
    {
        err=xQueueSend(queue1_handler,&key, portMAX_DELAY);
        if(err==pdPASS){
            printf("add success!\r\n");
        }
        
       
    
    
    }
    else if(key==KEY1_PRES){
            err=xSemaphoreGive(semp_handler);
            if(err==pdPASS){
                printf("success release\r\n");
            }
            else{
                printf("fail to release\r\n");
            }
        }

    vTaskDelay(10);


       

    }
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task2(void *pvParameters)
{
    (void)pvParameters;
    uint8_t key=0;
    BaseType_t err=0;
    QueueSetHandle_t memory_handler;
    while(1)
    {
    memory_handler=xQueueSelectFromSet(queueset_handler, portMAX_DELAY); /* 从队列集中选择一个已准备好的成员 */
    if(memory_handler==queue1_handler){
        xQueueReceive(queue1_handler, &key, 0); /* 从队列1接收数据 */
        printf("key: %d\r\n", key);
    }
    else if(memory_handler==semp_handler){
        xSemaphoreTake(semp_handler, 0); /* 从信号量接收数据 */
        printf("semaphore received\r\n");

    }
}
}

