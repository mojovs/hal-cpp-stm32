//
// Created by meng on 2024/5/14.
//

#include "wwdg.h"
#include "stm32f4xx_hal_wwdg.h"
#include "led.h"
#include "key.h"
#include "delay.h"
WWDG_HandleTypeDef wwdgHandleTypeDef;
u8 WWDG_CNT=0x7F;

void wwdg_init(u8 tr, u8 wr, u32 fprer) {

    wwdgHandleTypeDef.Instance=WWDG;
    wwdgHandleTypeDef.Init.Prescaler=fprer;
    wwdgHandleTypeDef.Init.Window=wr;
    wwdgHandleTypeDef.Init.Counter=tr;  //计数器

    HAL_WWDG_Init(&wwdgHandleTypeDef);
    __HAL_WWDG_ENABLE_IT(&wwdgHandleTypeDef,WWDG_IRQn);
}

void wwdg_test() {
    LED_0_VAL=0;
    wwdg_init(0x7F,0x5F,WWDG_PRESCALER_8);
    while(1){
        delay_ms(100);

    }
}

//使能引脚
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg){
    __HAL_RCC_WWDG_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn,2,3);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
}
void WWDG_IRQHandler(void){
    HAL_WWDG_IRQHandler(&wwdgHandleTypeDef);
}
void HAL_WWDG_WakeupCallback(WWDG_HandleTypeDef* hwwdg){
    HAL_WWDG_Refresh(&wwdgHandleTypeDef);
    LED_1_VAL=!LED_1_VAL;
}


