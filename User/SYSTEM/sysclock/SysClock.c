//
// Created by meng on 2023/11/20.
//

#include "SysClock.h"

void Stm32_Clock_Init(u32 plln, u32 pllm, u32 pllp, u32 pllq) {
    HAL_StatusTypeDef ret=HAL_OK;
    RCC_OscInitTypeDef rccOscInitType;
    RCC_ClkInitTypeDef rccClkInitType;
    //使能pwr时钟
    __HAL_RCC_PWR_CLK_ENABLE();
    //下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    //时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    //选择为外部时钟
    rccOscInitType.OscillatorType=RCC_OSCILLATORTYPE_HSE;
    rccOscInitType.HSEState=RCC_HSE_ON;
    //选择pll
    rccOscInitType.PLL.PLLState=RCC_PLL_ON;
    rccOscInitType.PLL.PLLSource=RCC_PLLSOURCE_HSE;
    rccOscInitType.PLL.PLLM=pllm;
    rccOscInitType.PLL.PLLN=plln;
    rccOscInitType.PLL.PLLP=pllp;
    rccOscInitType.PLL.PLLQ=pllq;
    ret= HAL_RCC_OscConfig(&rccOscInitType);
#ifndef CLION
    if(ret!=HAL_OK) while(1);
#endif
    ret=HAL_PWREx_EnableOverDrive();
#ifndef CLION
    if(ret!=HAL_OK) while(1);
#endif
    //用pll来作为systemclock且配置HCLK ,PCK1,2
    rccClkInitType.ClockType=RCC_CLOCKTYPE_SYSCLK |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    rccClkInitType.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;
    rccClkInitType.AHBCLKDivider=RCC_SYSCLK_DIV1;
    rccClkInitType.APB1CLKDivider=RCC_SYSCLK_DIV4;
    rccClkInitType.APB2CLKDivider=RCC_SYSCLK_DIV2;

    HAL_RCC_ClockConfig(&rccClkInitType,FLASH_LATENCY_5);//设置FLASH延时周期为5WS，也就是6个CPU周期。
#ifndef CLION
    if(ret!=HAL_OK) while(1);
#endif

}
