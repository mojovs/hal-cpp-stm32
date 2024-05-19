
extern "C" {
#include "sys.h"
#include "delay.h"
#include "Usart.h"
#include "../SYSTEM/led/led.h"
#include "../SYSTEM/key/key.h"
#include "exti.h"
#include "Iwdg.h"
#include "stm32f4xx_hal_iwdg.h"
#include "Wwdg.h"
#include "timer.h"
#include "pwm.h"
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
    pwm3_ch4_test();
    return 0;
}
