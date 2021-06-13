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
} LD_TypeDef;

typedef struct {
	LD_TypeDef LD_Yellow;
	LD_TypeDef LD_Green;
	LD_TypeDef LD_Red;
	LD_TypeDef LD_Blue;
	LD_TypeDef LD_White;
	CANBus *gseBus;
	CANBus *systemsBus;
} IFC_Test_PinStruct;

uint32_t ifc_test_logTime(void);
void IFC_TEST_orderTransition(STATE_ID);
void IFC_TEST_blinkLED(LD_TypeDef);
void IFC_TEST_tasks_init(IFC_Test_PinStruct*);

