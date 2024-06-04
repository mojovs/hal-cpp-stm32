//
// Created by meng on 2024/6/3.
//

#include "rtc.h"
#include "led.h"
#include "lcd.h"
#include "delay.h"

RTC_HandleTypeDef rtc_handler;

u8 rtc_init(void) {
    rtc_handler.Instance=RTC;
    rtc_handler.Init.HourFormat=RTC_HOURFORMAT_24;
    rtc_handler.Init.AsynchPrediv=0x7F;
    rtc_handler.Init.SynchPrediv=0xFF;
    rtc_handler.Init.OutPut=RTC_OUTPUT_DISABLE;
    rtc_handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    rtc_handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&rtc_handler)!=HAL_OK){
        return 2;
    }
    if(HAL_RTCEx_BKUPRead(&rtc_handler,RTC_BKP_DR0)!=0x5050){
        rtc_set_time(23,59,56,RTC_HOURFORMAT12_PM);
        rtc_set_date(15,12,27,7);
        HAL_RTCEx_BKUPWrite(&rtc_handler,RTC_BKP_DR0,0x5050); //已初始化标记
    }

    return 0;
}
void HAL_RTC_MSpInit(RTC_HandleTypeDef *hrtc){
    RCC_OscInitTypeDef rcc_osc_initer;
    RCC_PeriphCLKInitTypeDef rcc_periph_initer;
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess(); //取消备份区域保护

    rcc_osc_initer.OscillatorType=RCC_OSCILLATORTYPE_LSE;
    rcc_osc_initer.LSEState=RCC_LSE_ON;
    rcc_osc_initer.PLL.PLLState=RCC_PLL_NONE;
    HAL_RCC_OscConfig(&rcc_osc_initer);

    rcc_periph_initer.PeriphClockSelection=RCC_PERIPHCLK_RTC;   //设置总线时钟
    rcc_periph_initer.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periph_initer);
    __HAL_RCC_RTC_ENABLE();
}

void rtc_set_alarmA(u8 week,u8 hour,u8 min,u8 sec){

    RTC_AlarmTypeDef rtc_alarm;
    rtc_alarm.AlarmTime.Hours=hour;
    rtc_alarm.AlarmTime.Minutes=min;
    rtc_alarm.AlarmTime.Seconds=sec;
    rtc_alarm.AlarmTime.SubSeconds=0;
    rtc_alarm.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;

    rtc_alarm.AlarmMask=RTC_ALARMMASK_NONE;
    rtc_alarm.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    rtc_alarm.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;  //设置为星期
    rtc_alarm.AlarmDateWeekDay=week;

    rtc_alarm.Alarm=RTC_ALARM_A;

    //设置闹钟中断
    HAL_RTC_SetAlarm_IT(&rtc_handler,&rtc_alarm,RTC_FORMAT_BIN);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,1,2);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

}

HAL_StatusTypeDef rtc_set_time(u8 hour, u8 min, u8 second, u8 ampm) {
    RTC_TimeTypeDef rtc_time;
    rtc_time.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    rtc_time.Hours=hour;
    rtc_time.Minutes=min;
    rtc_time.Seconds=second;
    rtc_time.TimeFormat=ampm;
    rtc_time.StoreOperation=RTC_STOREOPERATION_RESET;

    return HAL_RTC_SetTime(&rtc_handler,&rtc_time,RTC_FORMAT_BIN);
}

HAL_StatusTypeDef rtc_set_date(u8 year, u8 month, u8 day, u8 week) {
    RTC_DateTypeDef rtc_date;
    rtc_date.Date=day;
    rtc_date.Year=year;
    rtc_date.Month=month;
    rtc_date.WeekDay=week;
    return HAL_RTC_SetDate(&rtc_handler,&rtc_date,RTC_FORMAT_BIN);
}
//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&rtc_handler);

}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{


}

//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&rtc_handler);
}

//RTC WAKE UP中断处理
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    LED_1_VAL=!LED_1_VAL;
}


void rtc_set_wakeup(u32 wksel, u16 cnt) {
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&rtc_handler,RTC_FLAG_WUTF);
    HAL_RTCEx_SetWakeUpTimer_IT(&rtc_handler,cnt,wksel);
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); //抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

void rtc_test(void) {
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    u8 tbuf[40];
    u8 t=0;
    LCD_Init();
    POINT_COLOR=RED;
    rtc_init();
    //选择spre 1hz 0x100，1s钟唤醒一次
    rtc_set_wakeup(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0);
    POINT_COLOR=RED;
    LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7");
    LCD_ShowString(30,70,200,16,16,"RTC TEST");
    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
    LCD_ShowString(30,110,200,16,16,"2016/1/13");
    while(1)
    {
        t++;
        if((t%10)==0)	//每100ms更新一次显示数据
        {
            HAL_RTC_GetTime(&rtc_handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
            sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
            LCD_ShowString(30,140,210,16,16,tbuf);
            HAL_RTC_GetDate(&rtc_handler,&RTC_DateStruct,RTC_FORMAT_BIN);
            sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date);
            LCD_ShowString(30,160,210,16,16,tbuf);
            sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.WeekDay);
            LCD_ShowString(30,180,210,16,16,tbuf);
        }
        if((t%20)==0)LED_0_VAL=!LED_0_VAL;	//每200ms,翻转一次LED0
        delay_ms(10);
    }
}

