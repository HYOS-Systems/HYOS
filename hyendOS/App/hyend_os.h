/*
 * ccdh.h
 *
 *  Created on: Jan 12, 2021
 *      Author: Bayram
 */
#pragma once

// Include Drivers
#include "connectivity/canTri/App/canTri.h"
#include "connectivity/sd/App/SDfileHandling.h"
#include "connectivity/xprintf/App/xprintf.h"
#include "hyendOS/OSVersioning/App/OSVersioning.h"
#include "peripherals/adc/ADS8688/App/ads8688.h"


// Define valid Initialization
#define INC_CCDH_H_

void voidMethod(void);

enum MICROCONTROLLER {
	NULL_MCU = 0,
	IFC,
	FSMS,
	RSMS,
	VAS,
	COMBINER
};

typedef struct {
	uint8_t number;
	void (*initHandle)(void);
	void (*canNVICHandle)(CANBus bus);
	void (*cyclic5HzHandle)(void);
	void (*cyclic10HzHandle)(void);
	void (*cyclic100HzHandle)(void);
	void (*cyclic1kHzHandle)(void);
	void (*cyclic10kHzHandle)(void);
} Microcontroller;

Microcontroller* microcontroller;

void initHyendOS(enum MICROCONTROLLER);

// Include Subsystems
#include "hyendOS/systems/rsms/rsms.h"
