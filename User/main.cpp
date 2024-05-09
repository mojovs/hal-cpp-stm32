extern "C" {
#include "sys.h"
#include "delay.h"
#include "Usart.h"
}

#include "led.h"
#include "key.h"
int main(void) {
    HAL_Init();
    Stm32_Clock_Init(360, 25, 2, 8);
    delay_init(180);
    Led led;
    Key key;
    u8 buffer[20] = "This is a test";
    int a = 0x12345678;
    Init_Usart2();
    Usart2_Enable_Recv();


    while (1) {
        //初始化的瞬间保持灭
        Usart2_Send(buffer,20);
        delay_ms(1000);

    }

    return 0;
}
