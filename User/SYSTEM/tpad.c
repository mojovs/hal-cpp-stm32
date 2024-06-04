//
// Created by meng on 2024/5/26.
//

#include "tpad.h"
#include "stm32f4xx_hal_tim.h"
#include "delay.h"
#include "tools.h"
vu16 tpad_default_val=0;
#define TPAD_ARR_MAX_VAL 0xFFFFFFFF //最大值
TIM_HandleTypeDef tim2_ch1_handler;

void tim2_ch1_cap_init(u32 arr, u16 psc) {
    //初始化时钟
    TIM_IC_InitTypeDef tim2_ch1_ic_handler;
    tim2_ch1_handler.Instance=TIM2;
    tim2_ch1_handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    tim2_ch1_handler.Init.Period=arr;
    tim2_ch1_handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    tim2_ch1_handler.Init.Prescaler=psc;
    HAL_TIM_IC_Init(&tim2_ch1_handler);

    tim2_ch1_ic_handler.ICFilter=0;
    tim2_ch1_ic_handler.ICPolarity=TIM_ICPOLARITY_RISING;
    tim2_ch1_ic_handler.ICPrescaler=TIM_ICPSC_DIV1;
    tim2_ch1_ic_handler.ICSelection=TIM_ICSELECTION_DIRECTTI;
    HAL_TIM_IC_ConfigChannel(&tim2_ch1_handler,&tim2_ch1_ic_handler,TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&tim2_ch1_handler,TIM_CHANNEL_1);
}

u8 tpad_init(u8 psc) {
    u16 buf[10];
    u16 temp;
    u8 j,i;
    tim2_ch1_cap_init(TPAD_ARR_MAX_VAL,psc-1);//设置分频系数
    for(i=0;i<10;i++)//连续读取10次
    {
        buf[i]=tpad_get_val();
        delay_ms(10);
    }
    bubble_sort(buf,10);
    //取其中8个数值进行平均
    for(i=2;i<8;i++)temp+=buf[i];//取中间的8个数据进行平均
    tpad_default_val=temp/6;
    if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
        return -1;//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
    return 1;
}

u8 tpad_reset() {
    GPIO_InitTypeDef  gpio_initer;
    //电容放电过程
    //引脚输出低电平
    gpio_initer.Pin=GPIO_PIN_5;
    gpio_initer.Alternate=GPIO_AF1_TIM2;
    gpio_initer.Speed=GPIO_SPEED_HIGH;
    gpio_initer.Pull=GPIO_PULLDOWN;
    gpio_initer.Mode=GPIO_MODE_OUTPUT_PP;   //推挽输出，增加放电能力
    HAL_GPIO_Init(GPIOA,&gpio_initer);

    //将gpioa_5下拉
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
    delay_ms(5);

    //将引脚设置为输入模式，检测输入
    gpio_initer.Mode=GPIO_MODE_AF_PP;   //复用推挽输入
    gpio_initer.Pull=GPIO_NOPULL;
    gpio_initer.Alternate=GPIO_AF1_TIM2;    //设置为pwm模式
    __HAL_TIM_CLEAR_IT(&tim2_ch1_handler,TIM_IT_CC1|TIM_IT_UPDATE); //CC1是通道1,update是持续捕获
    __HAL_TIM_SET_COUNTER(&tim2_ch1_handler,0);

    HAL_GPIO_Init(GPIOA,&gpio_initer);

    return 0;
}

u16 tpad_get_val() {
    tpad_reset();
    //等待CC11中断
    while (__HAL_TIM_GET_FLAG(&tim2_ch1_handler,TIM_FLAG_CC1)==RESET){
        //等待超时了
        if(__HAL_TIM_GET_COUNTER(&tim2_ch1_handler)>TPAD_ARR_MAX_VAL-500){
            return __HAL_TIM_GET_COUNTER(&tim2_ch1_handler) ;
        }
    }
    //等待到cc1，直接读取捕获到的值
    return HAL_TIM_ReadCapturedValue(&tim2_ch1_handler,TIM_CHANNEL_1);

}

u16 tpad_get_maxval(u8 n) {
    u16 temp=0;
    u16 res=0;
    u8 lcntnum=n*2/3;//至少2/3*n的有效个触摸,才算有效
    u8 okcnt=0;
    while(n--)
    {
        temp=tpad_get_val();//得到一次值
        if(temp>(tpad_default_val*5/4))okcnt++;//至少大于默认值的5/4才算有效
        if(temp>res)res=temp;
    }
    if(okcnt>=lcntnum)return res;//至少2/3的概率,要大于默认值的5/4才算有效
    else return 0;
}

u8 tpad_scan(u8 mode) {
    static u8 key_en=0;  //只有等于0的时候才能开始检测
    u8 res=0;
    u8 sample=3;    //采样次数，默认3次
    u16 ret=0;  //返回值`

    if(mode){
        sample=6;
        key_en=0;   //可以检测
    }
    ret = tpad_get_maxval(sample);
    // 在 默认电容充电 的4/3到10范围内
    if(ret>(tpad_default_val*4/3) &&ret<(tpad_default_val*10) ){
        if(!key_en) res=1;
        key_en=3;       //得3次循环之后，才可以继续返回1
    }
    if(key_en) key_en--;
    return res;
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim){
    GPIO_InitTypeDef gpio_initer;
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_initer.Pin=GPIO_PIN_5;
    gpio_initer.Mode=GPIO_MODE_AF_PP;
    gpio_initer.Pull=GPIO_NOPULL;   //浮空
    gpio_initer.Speed=GPIO_SPEED_HIGH;
    gpio_initer.Alternate=GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA,&gpio_initer);
}
