SDIO File Handling Library (With FATFS)

Compatible with STM32F4xx
Dependency to HAL_F4xx Library, FATFS (Middleware; ThirdParty)
Uses: xprintf; initialize used Libs first

HowTo init CubeMX (SDIO with FATFS)
1. Under Connectivity - SDIO, activate with SD 1 Bit Mode
2. Under System Core - GPIO - SDIO, set every pin Pull-Setting to Pull-Up
3. Under Middleware - FATFS, activate SD Card
	- Under "Set Defines" set the MAX_SS (Maximum Sector Size) to 4096
4. Generate and open Code (See document under "Lib/ReadMe.txt") (Ignore Warning of wrong FATFS parameter values)
5. Add code
	1. In main #include "sd/App/SDfileHandling.h"
	2. Somewhere after MX_SDIO_SD_Init() (but in UserCode) add initSDfileHandling();
	3. Note: Your Code can be deleted when re-generating the code with CubeMX. Read the UserCode entry under "Lib/ReadMe.txt" to prevent this.

PitFalls:
- Ensure that your SD Card is FAT32 Formatted. (Use Windows partition creation and formation tools)