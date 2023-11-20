//
// Created by meng on 2023/11/18.
//

#ifndef TEMPLATE_KEY_H
#define TEMPLATE_KEY_H


class Key {
public:
    Key();
    ~Key();
//读取到按键值
#define KEY_VAL_WK HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)
#define KEY_VAL_2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define KEY_VAL_0 HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)
#define KEY_VAL_1 HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)

//按键ID
    typedef enum _KEY_ID {
        KEY_ID_NONE,
        KEY_ID_WK,
        KEY_ID_0,
        KEY_ID_1,
        KEY_ID_2
    } KEY_ID;
    typedef enum _KEY_MODE {
        KEY_MODE_ONCE/*按一次，输出一次*/,
        KEY_MODE_CONTINIOUS/*可以一直按着输出*/
    } KEY_MODE;

/**
 * 初始化按键
 */
    void Key_Init(void);

/**
 * 按键扫描
 * @param mode KEY_MODE_RELEASE:松开后,才能按，KEY_MODE_LONG:一直按着，输出
 * @return 按键值
 */
    KEY_ID Key_Scan(KEY_MODE mode);
};

#endif //TEMPLATE_KEY_H
