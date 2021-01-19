Triple CAN Library

Compatible with STM32F4xx
Dependency to HAL_F4xx Library

HowTo init CubeMX (Baud Rate: 1000 MHz)
1. Clock and Baud Rate
	1. Set APB1 Clock to 32 MHz (under Clock Configuration)
	2. Activate all desired CAN Settings (Master or Slave, whatever it is) (under Pinout & Configuratio)
	3. For every CAN-"Parameter Settings":
	  - Set Prescaler to 4
	  - Set Time Quanta in Bit Segment 1 to 6
	  - Set Time Quanta in Bit Segment 2 to 1
	  - Set ReSynchronization Jump Width to 1
2. NVIC Settings
	1. For every CAN-"NVIC Settings" activate "CAN# RX0 interrupts"
	2. Under "System Core - NVIC" set Preemtion Priority of "CAN# RX0 interrupts" to something lower than 0. E.g.: 3.
	3. Note: NVIC settings can cause unforeseen behavior and errors. Read the NVIC entry under "Lib/ReadMe.txt".
3. Generate and open Code (See document under Lib/ReadMe.txt)
4. Add code
	1. In main #include "canTri/App/canTri.h"
	2. Somewhere after MX_CAN#_Init() (but in UserCode) add initCAN(&hcan#, desiredCAN_ID);
	3. Note: Your Code can be deleted when re-generating the code with CubeMX. Read the UserCode entry under "Lib/ReadMe.txt" to prevent this.

Send and Receive Messages
- You can only send messages of 8 bit length.
- Sending
	1. Pack all your data into an 8 bit uint8_t array. (Ensure that every useless byte is set to 0 to prevent yourself from doing errors)
	2. Send the Array with CAN#_SendMessage(uint8_t*);
- Receiving
	1. Declare an unsigned char[9] array to receive the message (a end-string symbol is added automaticly)
	2. In the stm32f4xx_it.c file within the respective CAN#_RX0_IRQHandler, receive the Message with CAN#_ReceiveMessage(unsigned char*);