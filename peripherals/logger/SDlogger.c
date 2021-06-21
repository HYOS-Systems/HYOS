/*
 * logger.c
 *
 *  Created on: Jun 3, 2021
 *      Author: Bayram
 */

#include "peripherals/logger/SDlogger.h"

#ifdef PERIPHERALS_LOGGER_LOGGER_H_

typedef struct {
	uint8_t i;
	uint16_t fileCounter;
	uint32_t dataCounter;
	uint32_t maxDataPerFile;

	uint8_t overheadLengthln;
	uint8_t overheadLength;

	char prefix[3];
	char fileName[8];
	char dataString[255];
} Logger_variables;

Logger_variables Logger;

void Logger_OpenFile() {
//	snprintf(Logger.fileName, 9, "%s%06u", Logger.prefix, Logger.fileCounter);
	snprintf(Logger.fileName, 9, "%s%06d", Logger.prefix, Logger.fileCounter);
	SDFH_openFile(Logger.fileName);
}

void Logger_CloseFile() {
	SDFH_closeFile();
}

void Logger_init(uint16_t maxNumberOfDataPerFile) {
	uint8_t dataStringLength = 6;
	uint8_t timeStampStringLength = 10;
	uint8_t seperatorLength = 3; // ,,\0
	uint8_t endOfLineLength = 1; // \n
	Logger.overheadLength = timeStampStringLength + seperatorLength;
	Logger.overheadLengthln = Logger.overheadLength + endOfLineLength;

	Logger.fileCounter = 0;
	Logger.dataCounter = 0;

	Logger.maxDataPerFile = maxNumberOfDataPerFile == 0 ? 1000 : maxNumberOfDataPerFile;
	Logger.prefix[0] = 'D';
	Logger.prefix[1] = 'T';
	Logger.prefix[2] = '\0';
	Logger_OpenFile();
}

void Logger_SetMaxDataPerFile(uint32_t maxData) {
	Logger.maxDataPerFile = maxData;
}

void Logger_NextFile() {
	Logger_CloseFile();
	Logger.fileCounter++;
	Logger_OpenFile();
	Logger.dataCounter = 0;
}

void Logger_DataAdded() {
	Logger.dataCounter++;
	if (Logger.dataCounter >= Logger.maxDataPerFile) {
		Logger_NextFile();
	}

}

void Logger_logData(const char *message, uint8_t messageLength, uint32_t time_stamp, uint16_t data) {
	uint8_t len = messageLength + Logger.overheadLengthln;

	snprintf(Logger.dataString, len, "%s,%010u,%06u\n", message, time_stamp, data);
	SDFH_writeToFile(Logger.dataString, len - 1);
	Logger_DataAdded();
}

void Logger_StartDataPackage(const char *message, uint8_t messageLength, uint32_t time_stamp) {
	uint8_t len = messageLength + Logger.overheadLength;

	snprintf(Logger.dataString, len, "%s,%010u,", message, time_stamp);
	SDFH_writeToFile(Logger.dataString, len - 1);
}

void Logger_EndDataPackage() {
	SDFH_writeToFile("\n", 1);
	Logger_DataAdded();
}

void Logger_logChars(char *chars, uint8_t charLength) {
	SDFH_writeToFile(chars, charLength);
}

void Logger_logNumbers(uint16_t *data, uint8_t dataLength) {
	uint8_t len = 8;
	for (Logger.i = 0; Logger.i < dataLength; Logger.i++) {
		snprintf(Logger.dataString, len, "%06u,", data[Logger.i]);
		SDFH_writeToFile(Logger.dataString, len - 1);
	}
}

#endif
