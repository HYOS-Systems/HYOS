/*
 * dataMap.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Bayram
 */

#pragma once
#include "hyendOS/hyend_os.h"

#define data_size 53

typedef struct {
	DATA_ID key;
	uint32_t value;
	uint32_t timestamp;
	DATA_STATUS update;
} DATA_FIELD;

DATA_FIELD DATA_LIST[data_size];

void MAP_init(void);
DATA_FIELD* MAP_getData(DATA_ID key);
void MAP_setData(DATA_ID key, uint32_t data, uint32_t timestamp, DATA_STATUS status);

uint32_t MAP_getDataToCANP(DATA_ID key, CANP_Data *data);
void MAP_setDataFromCANP(CANP_Data *data, uint32_t timestamp);
