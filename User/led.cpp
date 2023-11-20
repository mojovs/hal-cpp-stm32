//
// Created by meng on 2023/11/18.
//

#include "led.h"
extern "C" {
#include "sys.h"
}

void Led::Led_Init(void) {
    //使能时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpioInitType;
    //GPIO初始化
    gpioInitType.Mode=GPIO_MODE_OUTPUT_PP;
    gpioInitType.Pin=GPIO_PIN_0|GPIO_PIN_1;
    gpioInitType.Pull=GPIO_PULLUP;
    gpioInitType.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB,&gpioInitType);

    //初始化的瞬间保持灭
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB0置1
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB1置1
}

Led::Led() {
    Led_Init();
}

Led::~Led() {

}
