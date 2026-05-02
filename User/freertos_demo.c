#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"`
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"


/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 List_t                  TestList;           
        
 */
List_t                  TestList; 
ListItem_t              ListItem1;          
ListItem_t              ListItem2;         
ListItem_t              ListItem3;  

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
#define TASK2_PRIO      2                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
// #define TASK3_PRIO      4                   /* 任务优先级 */
// #define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
// TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
// void task3(void *pvParameters);             /* 任务函数 */
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
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
    // xTaskCreate((TaskFunction_t )task3,
    //             (const char*    )"task3",
    //             (uint16_t       )TASK3_STK_SIZE,
    //             (void*          )NULL,
    //             (UBaseType_t    )TASK3_PRIO,
    //             (TaskHandle_t*  )&Task3Task_Handler);

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
    // vListInitialise(&TestList);                 
    // vListInitialiseItem(&ListItem1);            
    // vListInitialiseItem(&ListItem2);            
    // vListInitialiseItem(&ListItem3);
    uint8_t task1_num=0;
    
    
    while(1)
    {
    // printf("/**************ַ**************/\r\n");
    // printf("\t\t\t\r\n");
    // printf("TestList\t\t0x%p\t\r\n", &TestList);
    // printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);
    // printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));
    // printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
    // printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
    // printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
    // printf("/*****************************************************/\r\n");
    // printf("KEY0!\r\n\r\n\r\n");
    taskENTER_CRITICAL(); /* 进入临界区 */
    printf("Task1 is running! %d\r\n", task1_num++);
    taskEXIT_CRITICAL();
    delay_ms(10);//不会实现任务调度，整个运行大于10ms，运行四五次


       

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
    uint32_t task2_num=0;
    
    while(1)
    {   taskENTER_CRITICAL(); /* 进入临界区 */
        printf("Task2 is running! %d\r\n", task2_num++);
        taskEXIT_CRITICAL();
        delay_ms(10);

    }
}
// void task3(void *pvParameters)
// {
//     uint8_t key = 0;
//     (void)pvParameters;

//     while(1)
//     {
//         key = key_scan(0);

//         if(key == KEY0_PRES)
//         {
//             vTaskSuspend(Task1Task_Handler);
//             lcd_show_string(10, 116, 220, 16, 16, "LED0: Suspended", BLUE);
//         }

//         if(key == KEY1_PRES)
//         {
//             vTaskResume(Task1Task_Handler);
//             lcd_show_string(10, 116, 220, 16, 16, "LED0: Resumed  ", BLUE);
//         }

//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }
