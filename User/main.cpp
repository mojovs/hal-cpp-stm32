extern "C" {
#include "sys.h"
#include "usart.h"
#include "delay.h"
}

#include "led.h"
#include "key.h"
int main(void){
    HAL_Init();
    Stm32_Clock_Init(360,25,2,8);
    delay_init(180);
    Led led;
    Key key;

    while(1){
        Key::KEY_ID keyId=key.Key_Scan(Key::KEY_MODE_CONTINIOUS);
        switch (keyId) {
            case Key::KEY_ID_NONE:
                LED_0_VAL=1;
                LED_1_VAL=1;
                break;
            case Key::KEY_ID_0:
                LED_0_VAL=!LED_0_VAL;
                break;
            case Key::KEY_ID_1:
                LED_1_VAL=!LED_1_VAL;
                break;
            case Key::KEY_ID_2:
                LED_0_VAL=!LED_0_VAL;
                LED_1_VAL=!LED_1_VAL;
                break;
        }

        delay_ms(10);

    }
}