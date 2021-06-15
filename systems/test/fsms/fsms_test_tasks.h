/*
 * ifc_tests_tasks.h
 *
 *  Created on: Jun 10, 2021
 *      Author: Bayram
 */

#pragma once

#include "hyendOS/hyend_os.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} FSMS_LD_TypeDef;

typedef struct {
	FSMS_LD_TypeDef LD_Yellow;
	FSMS_LD_TypeDef LD_Green;
	FSMS_LD_TypeDef LD_Red;
	FSMS_LD_TypeDef LD_Blue;
	FSMS_LD_TypeDef LD_White;
	CANBus *canBus;
} FSMS_Test_PinStruct;

uint32_t FSMS_TEST_logTime(void);
void FSMS_TEST_canHandle(void*);
void FSMS_TEST_blinkLED(FSMS_LD_TypeDef);
void FSMS_TEST_tasks_init(FSMS_Test_PinStruct*);

