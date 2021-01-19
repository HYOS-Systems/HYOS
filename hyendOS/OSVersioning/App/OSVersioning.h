#pragma once

#define OS_HAL_VERSION_F1_ 1
#define OS_HAL_VERSION_F4_ 8

/* USER define STM-Version BEGIN ----------------------------------------------*/
#define OS_HAL_VERSION_ OS_HAL_VERSION_F1_
/* USER define STM-Version END ------------------------------------------------*/

#if(OS_HAL_VERSION_ & OS_HAL_VERSION_F1_)
#include "stm32f1xx_hal.h"
#elif(OS_HAL_VERSION_ & OS_HAL_VERSION_F4_)
#include "stm32f4xx_hal.h"
#endif

#if defined(__STM32F1xx_HAL_UART_H)  || defined(__STM32F4xx_HAL_UART_H)
#define __STM32_UART_PERIPHERAL
#endif

#if defined(STM32F1xx_HAL_SD_H)  || defined(STM32F4xx_HAL_SD_H)
#define __STM32_SDIO_PERIPHERAL
#endif

#if OS_HAL_VERSION_ & OS_HAL_VERSION_F1_
#if defined(STM32F1xx_HAL_CAN_H)
#elif OS_HAL_VERSION_ & OS_HAL_VERSION_F4_
if defined(STM32F4xx_HAL_CAN_H)
#endif
#define __STM32_CAN_PERIPHERAL
#endif
