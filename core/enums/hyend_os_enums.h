/*
 * mcu_states.h
 *
 *  Created on: 23.02.2021
 *      Author: Bayram
 */

#pragma once

#define MCU_STATES_H_

// Header ==============================================================
typedef enum {
	NULL_MCU = 0,
	VAS = 1,
	RSMS = 2,
	FSMS = 3,
	IFC = 4,
	GSE = 5,
	MAX_MCU = 7 // 3 bit
} MICROCONTROLLER;

typedef enum {
	NULL_MESSAGE = 0,
	TRANSITION = 1,
//	STATUS = 3,
//	REQUEST_STATUS,
	DATA = 3,
	REQUEST_DATA,
	MAX_MESSAGE_TYPE = 7 // 3 bit
} MESSAGE_TYPE;

// Status ==============================================================
typedef enum {
	SYSTEM_OK = 0,
	VAS_OK = 32,
	RSMS_OK = 64,
	RSMS_SD_FAULT,
	FSMS_OK = 96,
	FSMS_SD_FAULT,
	IFC_OK = 128,
	IFC_SD_FAULT,
	GSE_OK = 160,
	MAX_STATUS_ERROR = 255 // 8 bit
} MCU_STATUS;

typedef enum {
	NULL_STATE = 0,
	IDLE,
	QUICK_START,
	TEST_IFC,
	TEST_FSMS,
	TEST_RSMS,
	TEST_VAS,
	FUELING,
	RDY_SET,
	FLIGHT,
	LANDED,
	STATE_ID_END
} STATE_ID; // 16 bit

// Data ==============================================================
typedef enum {
	DATA_OK,
	DATA_DEPRECATED,
	DATA_NOT_AVAILABLE,
	DATA_ERROR,
	DATA_STATUS_MAX = 15 // 4 bit
} DATA_STATUS;

typedef enum {
	SYSTEM_NULL_DATA = 0,
	MCU,
	VAS_NULL_DATA =  VAS << 9, // 512
	VAS_STATUS,
	VAS_GOPRO_1,
	VAS_GOPRO_2,
	VAS_GOPRO_3,
	VAS_GOPRO_4,
	VAS_GOPRO_5,
	VAS_PICAM,
	FSMS_NULL_DATA = FSMS << 9, // 1024
	FSMS_STATUS,
	FSMS_ACC1_X,
	FSMS_ACC1_Y,
	FSMS_ACC1_Z,
	FSMS_GYR1_X,
	FSMS_GYR1_Y,
	FSMS_GYR1_Z,
	FSMS_ACC2_X,
	FSMS_ACC2_Y,
	FSMS_ACC2_Z,
	FSMS_GYR2_X,
	FSMS_GYR2_Y,
	FSMS_GYR2_Z,
	FSMS_LAT,
	FSMS_LONG,
	FSMS_HEIGHT,
	FSMS_SPEED,
	FSMS_HEADING,
	FSMS_ROC,
	FSMS_GPS_STATUS,
	FSMS_GPS_NSAT,
	RSMS_NULL_DATA = RSMS << 9, // 1536
	RSMS_STATUS,
	RSMS_PRESSURE_1,
	RSMS_PRESSURE_2,
	RSMS_PRESSURE_3,
	RSMS_PRESSURE_4,
	RSMS_PT_1,
	RSMS_PT_2,
	RSMS_PT_3,
	RSMS_PT_4,
	RSMS_PT_5,
	RSMS_PT_6,
	RSMS_PT_7,
	RSMS_PT_8,
	IFC_NULL_DATA = IFC << 9, // 2048
	IFC_STATUS,
	IFC_DOWNLINK,
	GSE_NULL_DATA = GSE << 9, // 2560
	GSE_STATUS,
	MAX_DATA_ID = 4095 // 12 bit
} DATA_ID;

