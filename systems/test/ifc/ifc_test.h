/*
 * rsms.h
 *
 *  Created on: 17.01.2021
 *      Author: Bayram
 */
#pragma once

#include "hyendOS/hyend_os.h"

#define HYENDOS_SYSTEMS_IFC_TEST_H_

typedef struct {
	GPIO_TypeDef LD_Yellow_Port;
	uint16_t LD_Yellow_Pin;
	GPIO_TypeDef LD_Green_Port;
	uint16_t LD_Green_Pin;
	GPIO_TypeDef LD_Red_Port;
	uint16_t LD_Red_Pin;
	GPIO_TypeDef LD_Blue_Port;
	uint16_t LD_Blue_Pin;
	GPIO_TypeDef LD_White_Port;
	uint16_t LD_White_Pin;
} IFC_Test_PinStruct;

IFC_Test_PinStruct ifc_test_instance;
//void rsms_init(Microcontroller*);

