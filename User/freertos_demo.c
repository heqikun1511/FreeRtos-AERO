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
#include "event_groups.h"

EventGroupHandle_t event_group_handler; /* 事件组句柄 */
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
    event_group_handler=xEventGroupCreate();            /* 创建事件组 */
    if(event_group_handler!=NULL){
        printf("event group create success\r\n");

    }
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
   
    
    
    while(1)
    {
        key=key_scan(0);
        if(key==KEY0_PRES)
        {
            printf("KEY0_PRES\r\n");
            xEventGroupSetBits(event_group_handler,0x01); /* 设置事件组的第0位 */
        }
        else if(key==KEY1_PRES)
        {
            printf("KEY1_PRES\r\n");
            xEventGroupSetBits(event_group_handler,0x02); /* 设置事件组的第1位 */
        }
        else if(key==KEY2_PRES)
        {
            printf("KEY2_PRES\r\n");
            xEventGroupSetBits(event_group_handler,0x04); /* 设置事件组的第2位 */
        }
        vTaskDelay(10);
    
    

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
    QueueSetHandle_t memory_handler;
    EventBits_t evenbit=0;
    while(1)
    {
        memory_handler=xEventGroupWaitBits(event_group_handler,0x01||0x02||0x04,pdTRUE,pdTRUE,portMAX_DELAY); /* 等待事件组的第0、1、2位任意一个被置位 */
        printf("the position of the bit is %d\r\n",memory_handler);
    }
}

