//
// Created by meng on 2023/11/21.
//

#include "Usart.h"

UART_HandleTypeDef uart2_handle;
u16 USART_RX_STATUS = 0;      //uart接受状态标志
unsigned char buf_recv[RECV_SIZE];
unsigned char buf_send[SEND_SIZE];
u8 USART_RX_BUF[USART_REC_LEN];

//usart 中断跳转到这
void USART2_IRQHandler(void) {

    u32 timout=0;
    u32 maxDelay=0x1FFFF;
    HAL_UART_IRQHandler(&uart2_handle);
    //再重新开启中断
    //等待串口状态准备好
    while(HAL_UART_GetState(&uart2_handle)!=HAL_UART_STATE_READY){
        timout++;
        if(timout>maxDelay) break;
    }
    //等待接受中断完成
    timout=0;
    while(
    HAL_UART_Receive_IT(&uart2_handle, buf_recv, sizeof(buf_recv))!=HAL_OK){
        timout++;
        if(timout>maxDelay) break;
    }
}

// USART_RX_STATUS是一个16位的接受状态变量， 从高位到低为为
// 1位接受完成标志 + 1位接受到0x0d回车标志符 + 0-13位为数据位
// 在接受到回车0x0d，应该还要接受一个0x0a换行符，才会置接受完成位为1
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    //判断串口
    if (huart->Instance == USART2) {
        if ((USART_RX_STATUS & 0x8000) == 0) {    //已经接受到完成
            if (USART_RX_STATUS & 0x4000) { //已经接受到了0x0d
                if (buf_recv[0] != 0x0a) USART_RX_STATUS = 0;    //接受错误
                else USART_RX_STATUS |= 0x8000;   //重新开始
            } else {//还没有接受到0x0d回车符
                if (buf_recv[0] == 0x0d) USART_RX_STATUS |= 4000;    //这次接受到了回车符
                else {
                    //如果不是回车符，那么就是数据
                    USART_RX_BUF[USART_RX_STATUS&0x3FFF]=buf_recv[0];
                    USART_RX_STATUS++;
                    if(USART_RX_STATUS>(USART_REC_LEN-1)) USART_RX_STATUS=0;    //超界，错误重新发送
                }
            }

        }
    }

}

void Init_Usart2(void) {
    uart2_handle.Instance = USART2;
    uart2_handle.Init.BaudRate = 115200;  //设置波特率
    uart2_handle.Init.WordLength = USART_WORDLENGTH_8B;  //设置波特率
    uart2_handle.Init.StopBits = USART_STOPBITS_1;  //设置波特率
    uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;  //设置波特率
    uart2_handle.Init.Mode = USART_MODE_TX_RX;
    uart2_handle.Init.Parity = USART_PARITY_NONE; //不使用奇偶校验

    HAL_UART_Init(&uart2_handle);

}


void Usart2_Enable_Recv() {
    //使能接受中断
    HAL_UART_Receive_IT(&uart2_handle, buf_recv, sizeof(buf_recv));
}

void Usart2_Send(unsigned char *pdata, size_t size) {
    HAL_UART_Transmit(&uart2_handle, pdata, size, 1000);
}

void HAL_MspInit(void) {
    //初始化时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    //初始化gpio tx a9 rx a10
    GPIO_InitTypeDef gpioInitType;
    gpioInitType.Mode = GPIO_MODE_AF_PP;  //复用推挽
    gpioInitType.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpioInitType.Speed = GPIO_SPEED_HIGH;
    gpioInitType.Pull = GPIO_PULLUP;
    gpioInitType.Alternate = GPIO_AF7_USART2; //复用为usart2
    HAL_GPIO_Init(GPIOA, &gpioInitType);

    //设置中断
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

}
