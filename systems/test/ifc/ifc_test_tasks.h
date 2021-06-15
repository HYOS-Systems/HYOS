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
} IFC_LD_TypeDef;

typedef struct {
	IFC_LD_TypeDef LD_Yellow;
	IFC_LD_TypeDef LD_Green;
	IFC_LD_TypeDef LD_Red;
	IFC_LD_TypeDef LD_Blue;
	IFC_LD_TypeDef LD_White;
	CANBus *gseBus;
	CANBus *systemsBus;
} IFC_Test_PinStruct;

uint32_t IFC_TEST_logTime(void);
void IFC_TEST_canHandle(void*);
void IFC_TEST_orderTransition(STATE_ID);
void IFC_TEST_blinkLED(IFC_LD_TypeDef);
void IFC_TEST_tasks_init(IFC_Test_PinStruct*);

