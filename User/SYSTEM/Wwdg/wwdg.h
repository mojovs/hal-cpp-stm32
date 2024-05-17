//
// Created by meng on 2024/5/14.
//

#ifndef TEMPLATE_CPP_WWDG_H
#define TEMPLATE_CPP_WWDG_H
#include "sys.h"
void wwdg_init(u8 tr,u8 wr,u32 fprer);
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg);
void wwdg_test();


#endif //TEMPLATE_CPP_WWDG_H