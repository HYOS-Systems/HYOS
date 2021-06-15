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
} RSMS_LD_TypeDef;

typedef struct {
	RSMS_LD_TypeDef LD_Yellow;
	RSMS_LD_TypeDef LD_Green;
	RSMS_LD_TypeDef LD_Red;
	RSMS_LD_TypeDef LD_Blue;
	RSMS_LD_TypeDef LD_White;
	CANBus *canBus;
} RSMS_Test_PinStruct;

uint32_t RSMS_TEST_logTime(void);
void RSMS_TEST_canHandle(void*);
void RSMS_TEST_blinkLED(RSMS_LD_TypeDef);
void RSMS_TEST_tasks_init(RSMS_Test_PinStruct*);

