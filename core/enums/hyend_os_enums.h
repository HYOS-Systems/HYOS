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
	PCDU = 4,
	IFC = 5,
	GSE = 6,
	MAX_MCU = 7 // 3 bit
} MCU_ID;

typedef enum {
	NULL_MESSAGE = 0,
	TRANSITION = 1,
	DATA = 3,
	REQUEST_DATA,
	MAX_MESSAGE_TYPE = 7 // 3 bit
} MESSAGE_TYPE;

// Status ==============================================================
typedef enum {
	SYSTEM_OK = 0,
	VAS_OK = VAS << 5, // 32
	RSMS_OK = RSMS << 5, // 64
	RSMS_SD_FAULT,
	FSMS_OK = FSMS << 5, // 96
	FSMS_SD_FAULT,
	PCDU_OK = PCDU << 5, // 128
	IFC_OK = IFC << 5, // 160
	IFC_SD_FAULT,
	GSE_OK = GSE << 5, // 192
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
	DATA_OK = 0,
	DATA_DEPRECATED,
	DATA_NOT_AVAILABLE,
	DATA_ERROR,
	DATA_STATUS_MAX = 15 // 4 bit
} DATA_STATUS;

typedef enum {
	SYSTEM_NULL_DATA = 0,
	SYSTEM_STATUS,
	MCU,
	VAS_NULL_DATA = VAS << 9, // 512
	VAS_STATUS,
	VAS_GOPRO_1,
	VAS_GOPRO_2,
	VAS_GOPRO_3,
	VAS_GOPRO_4,
	VAS_GOPRO_5,
	VAS_PICAM,
	RSMS_NULL_DATA = RSMS << 9, // 1024
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
	FSMS_NULL_DATA = FSMS << 9, // 1536
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
	PCDU_NULL_DATA = PCDU << 9, // 2048
	PCDU_STATUS,
	IFC_NULL_DATA = IFC << 9, // 2560
	IFC_STATUS,
	IFC_DOWNLINK,
	GSE_NULL_DATA = GSE << 9, // 3072
	GSE_STATUS,
	MAX_DATA_ID = 4095 // 12 bit
} DATA_ID;

