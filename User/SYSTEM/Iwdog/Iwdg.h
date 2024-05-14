//
// Created by meng on 2024/5/11.
//

#ifndef TEMPLATE_CPP_IWDG_H
#define TEMPLATE_CPP_IWDG_H
#include "sys.h"

void iwdg_init(u8 prer, u16 rlr);
void iwdg_feed(void);

#endif //TEMPLATE_CPP_IWDG_H