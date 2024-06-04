//
// Created by meng on 2024/5/26.
//

#ifndef TEMPLATE_CPP_TPAD_H
#define TEMPLATE_CPP_TPAD_H

#include "sys.h"
extern vu16 tpad_default_val;

void tim2_ch1_cap_init(u32 arr,u16 psc);
u16 tpad_get_val();
/// 获取n次读取数值里里最大的值
/// \param n 读取次数
/// \return
u16 tpad_get_maxval(u8 n);
u8 tpad_init(u8 psc);
/// 将tpad电容放电
/// \return
u8 tpad_reset();
/// 扫描按键
/// \param mode 0：不支持连按 ;1：支持连按
/// \return
u8 tpad_scan(u8 mode);



#endif //TEMPLATE_CPP_TPAD_H