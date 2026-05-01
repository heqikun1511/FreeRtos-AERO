

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "./SYSTEM/usart/usart.h"

TIM_HandleTypeDef g_tim6_handle;      /* 定时器6句柄 */
TIM_HandleTypeDef g_tim7_handle;      /* 定时器7句柄 */

/**
 * @brief       基本定时器TIM7  定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim7_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM7_INT_CLK_ENABLE();                                      /* 使能TIM7时钟 */
    
    g_tim7_handle.Instance = BTIM_TIM7_INT;                          /* 通用定时器7 */
    g_tim7_handle.Init.Prescaler = psc;                              /* 分频 */
    g_tim7_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数器 */
    g_tim7_handle.Init.Period = arr;                                 /* 自动装载值 */
    g_tim7_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_tim7_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM7_INT_IRQn, 6, 0);  /* 设置中断优先级，抢占优先级6，子优先级0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM7_INT_IRQn);          /* 开启ITMx中断 */
    
    HAL_TIM_Base_Start_IT(&g_tim7_handle);           /* 使能定时器7和定时器7更新中断 */
    
    
}
void btim_tim6_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM6_INT_CLK_ENABLE();                                      /* 使能TIM6时钟 */
    
    g_tim6_handle.Instance = BTIM_TIM6_INT;                          /* 通用定时器6 */
    g_tim6_handle.Init.Prescaler = psc;                              /* 分频 */
    g_tim6_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数器 */
    g_tim6_handle.Init.Period = arr;                                 /* 自动装载值 */
    g_tim6_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_tim6_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM6_INT_IRQn, 6, 0);  /* 设置中断优先级，抢占优先级6，子优先级0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM6_INT_IRQn);          /* 开启ITMx中断 */
    
    HAL_TIM_Base_Start_IT(&g_tim6_handle);           /* 使能定时器6和定时器6更新中断 */
    
    
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM7_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim7_handle);
}

void BTIM_TIM6_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim6_handle);
}





/**
 * @brief       定时器更新中断回调函数
* @param        htim:定时器句柄指针
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&g_tim7_handle))
    {
        LED0_TOGGLE(); /* 翻转LED0 */
        printf("TIM7 Update Interrupt!!!!!!\r\n");
    }
    else if(htim==(&g_tim6_handle))
    {
        LED1_TOGGLE(); /* 翻转LED1 */
        printf("TIM6 Update Interrupt!!!!!!\r\n");
    }



}










