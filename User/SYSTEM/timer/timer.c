//
// Created by meng on 2024/5/17.
//

#include "timer.h"
#include "led.h"
#include "key.h"
#include "delay.h"

TIM_HandleTypeDef tim3_handle;
void timer3_init(u16 arr, u16 psc) {
    tim3_handle.Instance=TIM3;
    tim3_handle.Init.Prescaler=psc;
    tim3_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    tim3_handle.Init.Period=arr;
    tim3_handle.Init.CounterMode=TIM_COUNTERMODE_UP;

    HAL_TIM_Base_Init(&tim3_handle);
    HAL_TIM_Base_Start_IT(&tim3_handle);
}

//led1每500ms闪烁，led0每200ms闪烁
void timer3_test() {
    timer3_init(5000-1,9000-1); //配出500ms的定时
    while (1){
        LED_0_VAL=!LED_0_VAL;
        delay_ms(200);  //
    }
}

//使能时钟
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM3){
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn,1,3);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);

    }
}
//中断服务函数
void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&tim3_handle);
}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//    if(htim == &tim3_handle){
//        LED_1_VAL=!LED_1_VAL;
//    }
//}