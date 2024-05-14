
extern "C" {
#include "sys.h"
#include "delay.h"
#include "Usart.h"
#include "../SYSTEM/led/led.h"
#include "../SYSTEM/key/key.h"
#include "exti.h"
#include "Iwdg.h"
#include "stm32f4xx_hal_iwdg.h"
}
int main(void) {
    HAL_Init();
    Stm32_Clock_Init(360, 25, 2, 8);
    delay_init(180);
    Led_Init();
    Key_Init();
    u8 buffer[20] = "This is a test";
    int a = 0x12345678;
    Init_Usart2();
    Usart2_Enable_Recv();
    iwdg_init(IWDG_PRESCALER_64,500);
    LED_0_VAL=0;
    LED_1_VAL=0;

    while (1) {
        if(Key_Scan(KEY_MODE_CONTINIOUS)==KEY_ID_WK){

            iwdg_feed();
        }
        delay_ms(10);

    }

    return 0;
}
