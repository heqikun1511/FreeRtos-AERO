#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

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
void timer1_callback(TimerHandle_t xTimer);
void timer2_callback(TimerHandle_t xTimer);





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
TimerHandle_t timer1_handler; /* 定时器句柄 */
TimerHandle_t timer2_handler; /* 定时器句柄 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    timer1_handler = xTimerCreate("timer1", pdMS_TO_TICKS(1000), pdFALSE, (void *)1, timer1_callback); /* 创建单次定时器 */
    timer2_handler = xTimerCreate("timer2", pdMS_TO_TICKS(1000), pdTRUE, (void *)2, timer2_callback); /* 创建周期定时器 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}
void timer1_callback(TimerHandle_t xTimer)
{
    static uint32_t timer=0;
    printf("timer1_callback%lu\r\n",++timer);
}
void timer2_callback(TimerHandle_t xTimer)
{
    static uint32_t timer2=0;
    printf("timer2_callback%lu\r\n",++timer2);
    
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
            xTimerStart(timer1_handler,portMAX_DELAY); /* 启动定时器1 */
            xTimerStart(timer2_handler,portMAX_DELAY); /* 启动定时器2 */
        }
        else if(key==KEY1_PRES)
        {
            xTimerStop(timer1_handler,portMAX_DELAY); /* 停止定时器1 */
            xTimerStop(timer2_handler,portMAX_DELAY); /* 停止定时器2 */
       
        }
        vTaskDelay(10);
    }
}
    

    
