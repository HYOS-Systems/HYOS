/*
 * logger.c
 *
 *  Created on: Jun 3, 2021
 *      Author: Bayram
 */

#include "peripherals/logger/SDlogger.h"

#ifdef PERIPHERALS_LOGGER_LOGGER_H_

uint16_t fileCounter;
uint32_t dataCounter;
uint32_t maxDataPerFile;

uint8_t overheadLengthln;
uint8_t overheadLength;

char prefix[2];
char fileName[9];
char dataString[255];

void Logger_OpenFile() {
	snprintf(fileName, 9, "%s%06u", prefix, fileCounter);
	SDFH_openFile(fileName);
}

void Logger_CloseFile() {
	SDFH_closeFile();
}

void Logger_init(uint16_t maxNumberOfDataPerFile) {
	uint8_t dataStringLength = 5;
	uint8_t timeStampStringLength = 10;
	uint8_t seperatorLength = 4; // ,,\0
	uint8_t endOfLineLength = 1; // \n
	overheadLength = dataStringLength + timeStampStringLength + seperatorLength;
	overheadLengthln = overheadLength + endOfLineLength;

	fileCounter = 0;
	dataCounter = 0;

	maxDataPerFile = maxNumberOfDataPerFile == 0 ? 1000 : maxNumberOfDataPerFile;
	prefix[0] = 'D';
	prefix[1] = 'T';
	Logger_OpenFile();
}

void Logger_SetMaxDataPerFile(uint32_t maxData) {
	maxDataPerFile = maxData;
}

void Logger_NextFile() {
	Logger_CloseFile();
	fileCounter++;
	Logger_OpenFile();
	dataCounter = 0;
}

void Logger_DataAdded() {
	dataCounter++;
	if (dataCounter >= maxDataPerFile) {
		Logger_NextFile();
	}

}

void Logger_logData(const char *message, uint8_t messageLength, uint32_t time_stamp, uint16_t data) {
	uint8_t len = messageLength + overheadLengthln;

	snprintf(dataString, len, "%s,%010u,%06u\n", message, time_stamp, data);
	SDFH_writeToFile(dataString, len);
	Logger_DataAdded();
}

void Logger_StartDataPackage(const char *message, uint8_t messageLength, uint32_t time_stamp) {
	uint8_t len = messageLength + overheadLength;

	snprintf(dataString, len, "%s,%010u,", message, time_stamp);
	SDFH_writeToFile(dataString, len);
}

void LoggerE_ndDataPackage() {
	SDFH_writeToFile("\n", 2);
	Logger_DataAdded();
}

void Logger_logChars(char *chars, uint8_t charLength) {
	SDFH_writeToFile(chars, charLength);
}

#endif
