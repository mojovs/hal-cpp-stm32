//
// Created by meng on 2024/5/17.
//

#include "pwm.h"
#include "led.h"
#include "delay.h"

TIM_OC_InitTypeDef tim3_oc_init;
void tim3ch4_pwm_init(u16 presc, u16 arr) {
    tim3_handle.Instance=TIM3;
    tim3_handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    tim3_handle.Init.Period=arr;
    tim3_handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    tim3_handle.Init.Prescaler=presc;
    HAL_TIM_PWM_Init(&tim3_handle);
    // PWM 模式 1––在递增计数模式下，只要 TIMx_CNT<TIMx_CCR1，通道1便为有效状态，
    // 否则为无效状态。在递减计数模式下，只要 TIMx_CNT>TIMx_CCR1，通道 1 便为
    // 无效状态 (OC1REF=0)，否则为有效状态 (OC1REF=1)。
    tim3_oc_init.OCMode=TIM_OCMODE_PWM1;
    tim3_oc_init.Pulse=arr/2;   //占空比为一半
    tim3_oc_init.OCPolarity=TIM_OCPOLARITY_LOW; //
    HAL_TIM_PWM_ConfigChannel(&tim3_handle,&tim3_oc_init,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&tim3_handle,TIM_CHANNEL_4);
}

void TIM_SetTIM3Compare4(u32 compare) {
    TIM3->CCR4=compare;
}

void pwm3_ch4_test() {
    u8 dir=1;   //1递增模式
    u16 led0_pwm_val=0;
    tim3ch4_pwm_init(90 - 1, 500 - 1);//频率为90M/500*90=20KHz
    //通过调整比较值
    while (1){
        delay_ms(10);
        //调整占空比
        if(dir) led0_pwm_val++;
        else led0_pwm_val--;
        if(led0_pwm_val>300) dir=0; //500个时钟周期，大于300个周期，就转换成递减模式
        if(led0_pwm_val==0) dir=1;
        TIM_SetTIM3Compare4(led0_pwm_val);
    }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef  gpioInitTypeDef;
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpioInitTypeDef.Pin=GPIO_PIN_1;
    gpioInitTypeDef.Mode=GPIO_MODE_AF_PP;   //推挽
    gpioInitTypeDef.Pull=GPIO_PULLUP;
    gpioInitTypeDef.Speed=GPIO_SPEED_HIGH;
    gpioInitTypeDef.Alternate=GPIO_AF2_TIM3;    //PB1服用为TIM3-CH4
    HAL_GPIO_Init(GPIOB,&gpioInitTypeDef);

}
