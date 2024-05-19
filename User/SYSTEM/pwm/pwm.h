//
// Created by meng on 2024/5/17.
//

#ifndef TEMPLATE_CPP_PWM_H
#define TEMPLATE_CPP_PWM_H
#include "sys.h"
extern TIM_HandleTypeDef tim3_handle;
extern TIM_OC_InitTypeDef tim3_oc_init;

/// 初始化pwm3 ch4时钟
/// \param presc 分频系数，APB1 Timer是90M，
/// \param arr 自动装载值，用于设置多少个始终周期产生一下pwm
void tim3ch4_pwm_init(u16 presc, u16 arr);
/// 设置TIM3通道4的占空比
/// \param compare 比较值
void TIM_SetTIM3Compare4(u32 compare);
void pwm3_ch4_test();


#endif //TEMPLATE_CPP_PWM_H