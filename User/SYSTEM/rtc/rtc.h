//
// Created by meng on 2024/6/3.
//

#ifndef TEMPLATE_CPP_RTC_H
#define TEMPLATE_CPP_RTC_H
#include "sys.h"
extern RTC_HandleTypeDef rtc_handler;
u8 rtc_init(void);
HAL_StatusTypeDef rtc_set_time(u8 hour,u8 min,u8 second,u8 ampm);
HAL_StatusTypeDef rtc_set_date(u8 year,u8 month,u8 day,u8 week);

void rtc_set_alarmA(u8 week,u8 hour,u8 min,u8 sec);
void rtc_set_wakeup(u32 wksel,u16 cnt);
void rtc_test(void);


#endif //TEMPLATE_CPP_RTC_H