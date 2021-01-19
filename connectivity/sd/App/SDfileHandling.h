/*
 * dataHandling.h
 *
 *  Created on: 06.10.2020
 *      Author: Bayram
 */
#pragma once

#include <hyendOS/OSVersioning/App/OSVersioning.h>

#ifdef __STM32_SDIO_PERIPHERAL

#ifndef INC_FILEHANDLING_H_
#define INC_FILEHANDLING_H_
#endif /* INC_FILEHANDLING_H_ */

#include <stdio.h>
#include "xprintf/App/xprintf.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "ffconf.h"
#include "fatfs.h"
//#include "File_Handling.h"

extern FIL file;
extern UINT state;

FRESULT initSDfileHandling(void);

void getNumberLength(uint16_t*, uint16_t);

void getFileNameLength(uint16_t*, uint16_t);

void getFileName(char*, uint16_t);

void getDataLength(uint16_t*, uint16_t, uint16_t);

void packData(char*, uint16_t, uint16_t);

FRESULT SDFH_openFile(char*);

FRESULT SDFH_closeFile(void);

FRESULT SDFH_writeToFile(char*, uint16_t);

FRESULT SDFH_writeSingle(char*, char*, uint16_t);

void SDFH_writeTest(void);

void SDFH_normalOperation(void);

#endif // __STM32_SDIO_PERIPHERAL

