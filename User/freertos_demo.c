

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./FreeRTOSConfig.h"
#include "BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
StaticTask_t            StartTask_TCB;      /* 任务控制块 */
StackType_t             StartTask_Stack[START_STK_SIZE]; /* 任务堆栈 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
StaticTask_t            Task1Task_TCB;      /* 任务控制块 */
StackType_t             Task1Task_Stack[TASK1_STK_SIZE]; /* 任务堆栈 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
StaticTask_t            Task2Task_TCB;      /* 任务控制块 */
StackType_t             Task2Task_Stack[TASK2_STK_SIZE]; /* 任务堆栈 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      4                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
StaticTask_t            Task3Task_TCB;      /* 任务控制块 */
StackType_t             Task3Task_Stack[TASK3_STK_SIZE]; /* 任务堆栈 */
void task3(void *pvParameters);             /* 任务函数 */

StaticTask_t idle_task_tcb;                 /* 空闲任务控制块 */
StackType_t  idle_task_stack[configMINIMAL_STACK_SIZE]; /* 空闲任务堆栈 */
StaticTask_t timer_task_tcb;                /* 定时器任务控制块 */
StackType_t  timer_task_stack[configTIMER_TASK_STACK_DEPTH]; /* 定时器任务堆栈 */

/* 空闲任务和定时器任务内存分配函数 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
    {
        *ppxIdleTaskTCBBuffer = &idle_task_tcb; /* 将空闲任务控制块的地址传递给FreeRTOS */
        *ppxIdleTaskStackBuffer = idle_task_stack; /* 将空闲任务堆栈的地址传递给FreeRTOS */
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /* 将空闲任务堆栈大小传递给FreeRTOS */
    }
   
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
    {
        *ppxTimerTaskTCBBuffer = &timer_task_tcb; /* 将定时器任务控制块的地址传递给FreeRTOS */
        *ppxTimerTaskStackBuffer = timer_task_stack; /* 将定时器任务堆栈的地址传递给FreeRTOS */
        *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; /* 将定时器任务堆栈大小传递给FreeRTOS */
    }
/******************************************************************************************************/

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    lcd_show_string(10, 10, 220, 32, 32, "FreeRTOS", RED);
    lcd_show_string(10, 47, 220, 24, 24, "LED Blink Demo", RED);
    lcd_show_string(10, 76, 220, 16, 16, "LED0: 500ms", BLUE);
    lcd_show_string(10, 96, 220, 16, 16, "LED1: 200ms", BLUE);
    lcd_show_string(10, 116, 220, 16, 16, "KEY0: Delete Task1", BLUE);
    StartTask_Handler = xTaskCreateStatic(
        (TaskFunction_t )start_task,            /* 任务函数 */
        (const char*    )"start_task",          /* 任务名称 */
        (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
        (void*          )NULL,                  /* 传入给任务函数的参数 */
        (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
        (StackType_t*   )StartTask_Stack,       /* 静态分配的堆栈缓冲区 */
        (StaticTask_t*  )&StartTask_TCB         /* 静态分配的任务控制块缓冲区 */
    );

   vTaskStartScheduler();//开启任务调度器，开始执行任务
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区,作用是保护临界资源 ，暂停任务 */
    /* 创建任务1 */
    Task1Task_Handler = xTaskCreateStatic((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (StackType_t*   )Task1Task_Stack,
                (StaticTask_t*  )&Task1Task_TCB);
    /* 创建任务2 */
    Task2Task_Handler = xTaskCreateStatic((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (StackType_t*   )Task2Task_Stack,
                (StaticTask_t*  )&Task2Task_TCB);
    Task3Task_Handler = xTaskCreateStatic((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (StackType_t*   )Task3Task_Stack,
                (StaticTask_t*  )&Task3Task_TCB);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区，恢复任务 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task1(void *pvParameters)
{
    (void)pvParameters;
    
    while(1)
    {
        LED0_TOGGLE();                          /* LED0每500ms翻转一次 */
        vTaskDelay(pdMS_TO_TICKS(500));
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
    
    while(1)
    {
        LED1_TOGGLE();                          /* LED1每200ms翻转一次 */
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
//判断KEY是否按下，按下则删除任务
void task3(void *pvParameters)
{   uint8_t key=0;
    (void)pvParameters;
    
    while(1)
    {
        key=key_scan(0);
        if(key==KEY1_PRES && Task1Task_Handler != NULL){
            vTaskDelete(Task1Task_Handler);
            Task1Task_Handler = NULL;
            LED0(1);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
