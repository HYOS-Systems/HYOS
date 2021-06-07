/*
 * dataHandling.h
 *
 *  Created on: 06.10.2020
 *      Author: Bayram
 */
#pragma once

#include "drivers/hyend_os_drivers.h"

#ifdef __STM32_SDIO_PERIPHERAL

#define INC_DATAHANDLING_H_

#include <stdio.h>
#include "ff.h"
#include "ff_gen_drv.h"
#include "ffconf.h"
#include "fatfs.h"
//#include "File_Handling.h"

extern FIL file;
extern UINT state;

FRESULT initSDfileHandling(void);

void SDFH_getFileNameLength(uint8_t*, uint16_t);

void SDFH_getFileName(char*, uint16_t);

void SDFH_getDataLength(uint8_t*, uint16_t, uint16_t);

void SDFH_packData(char*, uint16_t, uint16_t);

FRESULT SDFH_openFile(char*);

FRESULT SDFH_closeFile(void);

FRESULT SDFH_writeToFile(char*, uint16_t);

FRESULT SDFH_writeIntToFile(int32_t);

FRESULT SDFH_writeSingle(char*, char*, uint16_t);

void SDFH_writeTest(void);

void SDFH_normalOperation(void);

#endif // __STM32_SDIO_PERIPHERAL

