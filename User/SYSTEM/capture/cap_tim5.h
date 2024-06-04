//
// Created by meng on 2024/5/21.
//

#ifndef TEMPLATE_CPP_CAP_TIM5_H
#define TEMPLATE_CPP_CAP_TIM5_H
#include "sys.h"

extern TIM_HandleTypeDef tim5_handle;
void tim5_ch1_init(u32 arr,u16 psc);
/// 测试
void tim5_ch1_cap_test();

#endif //TEMPLATE_CPP_CAP_TIM5_H