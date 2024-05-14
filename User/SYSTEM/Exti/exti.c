//
// Created by meng on 2024/5/9.
//

#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"

void exti_init(void) {
    GPIO_InitTypeDef gpioInitTypeDef;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    gpioInitTypeDef.Pin=GPIO_PIN_0;
    gpioInitTypeDef.Mode=GPIO_MODE_IT_RISING;   //上升沿采样
    gpioInitTypeDef.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &gpioInitTypeDef);

    gpioInitTypeDef.Pin=GPIO_PIN_13;
    gpioInitTypeDef.Mode=GPIO_MODE_IT_FALLING;   //上升沿采样
    gpioInitTypeDef.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &gpioInitTypeDef);

    gpioInitTypeDef.Pin=GPIO_PIN_2|GPIO_PIN_3;
    HAL_GPIO_Init(GPIOH, &gpioInitTypeDef);

    //设置中断优先级
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);
    //使能中断
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI0_IRQHandler(){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}
void EXTI2_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
void EXTI3_IRQHandler(){

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
void EXTI15_10_IRQHandler(){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    //消抖
    delay_ms(100);
    switch (GPIO_Pin) {
        case GPIO_PIN_0:
            //检测到了开机键
            if(KEY_VAL_WK){
                LED_1_VAL=!LED_0_VAL;
                LED_0_VAL=!LED_1_VAL;
            }
            break;
        case GPIO_PIN_2:
            if(KEY_VAL_1 ==0){
                LED_1_VAL=!LED_1_VAL;
            }
            break;
        case GPIO_PIN_3:
            if(KEY_VAL_0 ==0){
                LED_0_VAL=!LED_0_VAL;
                LED_1_VAL=!LED_1_VAL;
            }
            break;
        case GPIO_PIN_13:
            if(KEY_VAL_2 ==0){
                LED_0_VAL=!LED_0_VAL;
            }
            break;
    }

}
