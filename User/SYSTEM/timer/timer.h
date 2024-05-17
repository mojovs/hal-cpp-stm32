//
// Created by meng on 2024/5/17.
//

#ifndef TEMPLATE_CPP_TIMER_H
#define TEMPLATE_CPP_TIMER_H
#include "sys.h"
#include "stm32f4xx_hal.h"
extern TIM_HandleTypeDef timHandleTypeDef;
void timer3_init(u16 arr, u16 psc);
void timer3_test();


#endif //TEMPLATE_CPP_TIMER_H