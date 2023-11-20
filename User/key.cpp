//
// Created by meng on 2023/11/18.
//
#include "key.h"
extern "C" {
#include "sys.h"
#include "delay.h"
}

void Key::Key_Init(void) {
    //开启时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    //初始化gpioA
    GPIO_InitTypeDef gpioInitTypeDef;
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Pull = GPIO_PULLDOWN;
    gpioInitTypeDef.Speed = GPIO_SPEED_HIGH;
    gpioInitTypeDef.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOA, &gpioInitTypeDef);

    //初始化gpioc13 key2
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Pull = GPIO_PULLUP;
    gpioInitTypeDef.Speed = GPIO_SPEED_HIGH;
    gpioInitTypeDef.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &gpioInitTypeDef);

    //初始化gpio h 2 3   key1 key0
    gpioInitTypeDef.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOH, &gpioInitTypeDef);
}

Key::KEY_ID Key::Key_Scan(Key::KEY_MODE mode) {
    static u8 last_key_down = 0;   // 记录上次按键按下没
    if(mode == KEY_MODE_CONTINIOUS) last_key_down=0; //连续模式

    // 模式：上次没按，这次有按键，才能有输出
    if (!last_key_down && (KEY_VAL_0 == 0 || KEY_VAL_1 == 0 || KEY_VAL_2 == 0 || KEY_VAL_WK == 1)) {
        delay_ms(10);
        last_key_down = 1;  //设置为按过
        if (KEY_VAL_0 == 0) {
            return KEY_ID_0;
        } else if (KEY_VAL_1 == 0) {
            return KEY_ID_1;
        } else if (KEY_VAL_2 == 0) {
            return KEY_ID_2;
        } else if (KEY_VAL_WK == 1) {
            return KEY_ID_WK;
        }
    }else if(KEY_VAL_0 == 1 || KEY_VAL_1 == 1 || KEY_VAL_2 == 1 || KEY_VAL_WK == 0){
        last_key_down=0;
    }
    return KEY_ID_NONE;
}

Key::Key() {
    Key_Init();

}

Key::~Key() {

}
