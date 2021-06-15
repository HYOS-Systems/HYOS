/*
 * logger.h
 *
 *  Created on: Jun 3, 2021
 *      Author: Bayram
 */

#pragma once

#include "connectivity/sd/SDfileHandling.h"

#ifdef INC_DATAHANDLING_H_
#define PERIPHERALS_LOGGER_LOGGER_H_

void Logger_init(uint16_t maxNumberOfDataPerFile);

void Logger_logData(const char *message, uint8_t messageLength, uint32_t time_stamp, uint16_t data);

void Logger_StartDataPackage(const char *message, uint8_t messageLength, uint32_t time_stamp);
void Logger_logChars(char *chars, uint8_t charLength);
void Logger_EndDataPackage();
#endif
