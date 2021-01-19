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

HowTo write.
- Write single Data into own file:
	1. Use method SDFH_writeSingle(char*, char*, uint16_t)
		char* FileName (Maximal length is 8 chars)
		char* data
		uint16_t length of data
- Write multiple data into same file:
	1. Use method SDFH_openFile(char*), argument is FileName with maximal length of 8 chars
	2. Write as often as you wish with SDFH_writeToFile(char*, uint16_t) with the arguments as the data and the array length of the data
	3. When finished, use SDFH_closeFile() to close the file

PitFalls:
- Ensure that your SD Card is FAT32 Formatted. (Use Windows partition creation and formation tools)