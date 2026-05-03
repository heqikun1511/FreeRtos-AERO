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
#include "semphr.h"
#include "FreeRTOSConfig.h"




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




 QueueHandle_t   key_queue;      /* 队列句柄 */
 QueueHandle_t   big_data;
 char buff[100]={"大数组drthdydutduryfuyt"};
/******************************************************************************************************/
    

 QueueHandle_t semphore_handle;      /* 信号量句柄 */
void freertos_demo(void)
{
    semphore_handle=xSemaphoreCreateBinary();
    if(semphore_handle!=NULL){
        printf("success!\r\n");
    }
    else{
        printf("fail!\r\n");
    }
    
    // }
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
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
   
    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(NULL);              /* 删除自身，避免任务函数返回 */
}

/**申请释放内存*/
 
void task1(void *pvParameters)
{
    (void)pvParameters;
    uint8_t key_num=0,t=0;
  
  
   
    while(1)
    {
        key_num=key_scan(0);
        uint8_t* buffer=NULL;
        if(key_num==KEY0_PRES){

        buffer=pvPortMalloc(30);
        if(buffer!=NULL){
            printf("malloc success!\r\n");
        }
        else{
            printf("malloc fail!\r\n");
        }

            }
        else if(key_num==KEY1_PRES){
            vPortFree(buffer);
            if(buffer!=NULL){
                printf("free success!\r\n");
            }
           
             



        }
        if(t++>50){
            t=0;
            printf("remain heap size:%d\r\n",xPortGetFreeHeapSize());
        }
        }
 


    vTaskDelay(10);


       

    }



 
