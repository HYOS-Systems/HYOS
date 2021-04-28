/*
 * mcu_states.h
 *
 *  Created on: 23.02.2021
 *      Author: Bayram
 */

#pragma once

#define MCU_STATES_H_

typedef enum {
	NULL_MESSAGE = 0,
	TRANSITION = 1,
	STATUS = 3,
	REQUEST_STATUS,
	DATA,
	REQUEST_DATA,
	MAX_TYPE = 7
} MESSAGE_TYPE;

typedef enum {
	NULL_DATA = 0,
	MCU,
	GOPRO_1,
	GOPRO_2,
	GOPRO_3,
	GOPRO_4,
	GOPRO_5,
	PICAM,
	IMU_X,
	IMU_Y,
	IMU_Z,
	IMU_VX,
	IMU_VY,
	IMU_VZ,
	PRESSURE_1,
	PRESSURE_2,
	PRESSURE_3,
	PT_1,
	PT_2,
	PT_3,
	SD,
	CAN,
	CAN_1,
	CAN_2,
	CAN_3,
	MAX_ID = 4095
} DATA_ID;

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
} STATE_ID;

typedef enum {
	NULL_MCU = 0,
	VAS,
	RSMS,
	FSMS,
	IFC,
	GSE,
	MAX_MCU = 7
} MICROCONTROLLER;

