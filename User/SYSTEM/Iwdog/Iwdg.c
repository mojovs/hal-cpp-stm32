//
// Created by meng on 2024/5/11.
//

#include "Iwdg.h"
#include "stm32f4xx_hal_iwdg.h"

IWDG_HandleTypeDef iwdgHandleTypeDef;
void iwdg_init(u8 prer, u16 rlr) {
    iwdgHandleTypeDef.Instance=IWDG;
    iwdgHandleTypeDef.Init.Prescaler=prer;
    iwdgHandleTypeDef.Init.Reload=rlr;
    HAL_IWDG_Init(&iwdgHandleTypeDef);
    __HAL_IWDG_START(&iwdgHandleTypeDef);
}
void iwdg_feed(void){
    HAL_IWDG_Refresh(&iwdgHandleTypeDef);
}
