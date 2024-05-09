//
// Created by meng on 2023/11/20.
//

#ifndef TEMPLATE_CPP_SYSCLOCK_H
#define TEMPLATE_CPP_SYSCLOCK_H
extern "C" { ;
#include "stm32f4xx.h"
#include "sys.h"
}

class SysClock {
public:
    static void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);//时钟系统配置

};


#endif //TEMPLATE_CPP_SYSCLOCK_H
