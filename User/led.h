//
// Created by meng on 2023/11/18.
//

#ifndef TEMPLATE_LED_H
#define TEMPLATE_LED_H

class Led {
public:
    Led();

    virtual ~Led();

#define LED_0_VAL PBout(1)  //led0值
#define LED_1_VAL PBout(0)  //led1值

/**
 * 初始化led
 */
    void Led_Init(void);

};

#endif //TEMPLATE_LED_H