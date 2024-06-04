//
// Created by meng on 2024/5/21.
//

#include "cap_tim5.h"
#include "led.h"
#include "key.h"u
#include "delay.h"

TIM_HandleTypeDef tim5_handle;
/// tim5通道1 捕获标志
/// 1位捕获未完成标志 + 1捕获到高电平 + 6位捕获次数
u8 tim5ch1_cap_status=0;
u32 tim5ch1_cap_val=0; //通道
void tim5_ch1_init(u32 arr, u16 psc) {
    tim5_handle.Instance=TIM5;
    tim5_handle.Init.Prescaler=psc;
    tim5_handle.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数
    tim5_handle.Init.Period=arr;
    tim5_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&tim5_handle);  //初始化

    TIM_IC_InitTypeDef tim5_ch1_cfg;
    tim5_ch1_cfg.ICFilter=0;

    HAL_TIM_IC_ConfigChannel(&tim5_handle,&tim5_ch1_cfg,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&tim5_handle,TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&tim5_handle,TIM_IT_UPDATE);

}

void tim5_ch1_cap_test() {

    long long temp;
    tim5_ch1_init(0xFFFFFFFF,90-1); //分频系数
    while (1){
        delay_ms(10);
        if(tim5ch1_cap_status&0X80)        //成功捕获到了一次高电平
        {
            temp=tim5ch1_cap_status&0X3F; //获取溢出次数
            temp*=0XFFFFFFFF;		 	    //溢出时间总和
            temp+=tim5ch1_cap_val;      //得到总的高电平时间
            tim5ch1_cap_status=0;          //开启下一次捕获
        }
    }

}

//
//void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim){
//    __HAL_RCC_TIM5_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    GPIO_InitTypeDef gpioInitTypeDef;
//    gpioInitTypeDef.Pin=GPIO_PIN_0;
//    gpioInitTypeDef.Mode=GPIO_MODE_AF_PP;
//    gpioInitTypeDef.Pull=GPIO_PULLDOWN;
//    gpioInitTypeDef.Speed=GPIO_SPEED_HIGH;
//    gpioInitTypeDef.Alternate=GPIO_AF2_TIM5;
//    HAL_GPIO_Init(GPIOA,&gpioInitTypeDef);
//
//    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);
//    HAL_NVIC_EnableIRQ(TIM5_IRQn);
//
//}

void TIM5_IRQHandler(void){
    HAL_TIM_IRQHandler(&tim5_handle);
}

//定时器溢出 回调函数tim5ch1_cap_status 为溢出次数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //开始捕捉
    if((tim5ch1_cap_status &0x80) ==0){
        //如果捕捉到高电平
        if(tim5ch1_cap_status &0x40){
            //捕捉高电平的时间太长了
            if((tim5ch1_cap_status &0x3F)==0x3F){
                tim5ch1_cap_status|=0x80;   //成功捕捉一次
                tim5ch1_cap_val=0xFFFFFFFF;
            }else{
                tim5ch1_cap_status++;   //捕捉计数增加
            }
        }
    }
}
// 定时器输入捕获中断处理 回调函数，
//
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if((tim5ch1_cap_status&0x80)==0){
        //捕捉上升沿
        if(tim5ch1_cap_status&0x40){
            tim5ch1_cap_status|=0x80; //标记成功捕获一次高电平
            tim5ch1_cap_val= HAL_TIM_ReadCapturedValue(&tim5_handle,TIM_CHANNEL_1);
            TIM_RESET_CAPTUREPOLARITY(&tim5_handle,TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&tim5_handle,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);
        }
        //捕捉下降沿
        else{
            //清空标志和捕获值
            tim5ch1_cap_status=0;
            tim5ch1_cap_val=0;
            tim5ch1_cap_status|=0x40; //捕获到上升
            __HAL_TIM_DISABLE(&tim5_handle);
            __HAL_TIM_SET_COUNTER(&tim5_handle,0);

            TIM_RESET_CAPTUREPOLARITY(&tim5_handle,TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&tim5_handle,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);
            __HAL_TIM_ENABLE(&tim5_handle);
        }
    }
}