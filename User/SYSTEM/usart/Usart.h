//
// Created by meng on 2023/11/21.
//

#ifndef TEMPLATE_CPP_USART_H
#define TEMPLATE_CPP_USART_H
#include "sys.h"
#include "stm32f4xx_hal_uart.h"
#define SEND_SIZE 200
#define RECV_SIZE 200
#define USART_REC_LEN 200

extern UART_HandleTypeDef uart2_handle;
extern UART_HandleTypeDef *p_uart2_handle;
extern u16 USART_RX_STATUS;      //uart接受状态标志
extern u8 USART_RX_BUF[USART_REC_LEN];
//回调函数
void USART2_IRQHandler(void);
void Usart2_Send(unsigned char *pdata, size_t size);
void HAL_MspInit(void);

//初始化usart
    void Init_Usart2(void);
    void Usart2_Enable_Recv();
    void Usart2_Send(unsigned char *pdata, size_t size);


#endif //TEMPLATE_CPP_USART_H
