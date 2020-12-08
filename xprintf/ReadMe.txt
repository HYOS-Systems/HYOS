xprintf Library
for easy print to UART

Compatible with STM32F4xx
Dependency to HAL_F4xx Library

HowTo init in Code
1. Make sure, at least one UART is interfaced under Connectivity in CubeMX
2. Add code
	1. In main #include "xprintf/App/xprintf.h"
	2. Somewhere after MX_USART#_UART_Init(); (but in UserCode) add initUART(&huart#); (The #-Symbol refers to the desired UART number)
	3. Note: Your Code can be deleted when re-generating the code with CubeMX. Read the UserCode entry under "Lib/ReadMe.txt" to prevent this.