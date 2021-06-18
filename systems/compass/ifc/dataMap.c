/*
 * dataMap.c
 *
 *  Created on: Jun 18, 2021
 *      Author: Bayram
 */

#include "systems/compass/ifc/dataMap.h"

union {
	uint16_t i;
	DATA_FIELD *currentData;
} Map;

void MAP_setData(DATA_ID key, uint32_t data, uint32_t timestamp, DATA_STATUS status) {
	for (Map.i = 0; Map.i < data_size; Map.i++) {
		if (DATA_LIST[Map.i].key == key) {
			DATA_LIST[Map.i].value = data;
			DATA_LIST[Map.i].update = status;
			DATA_LIST[Map.i].timestamp = timestamp;
			break;
		}
	}
}

DATA_FIELD* MAP_getData(DATA_ID key) {
	for (Map.i = 0; Map.i < data_size; Map.i++) {
		if (DATA_LIST[Map.i].key == key) {
			DATA_LIST[Map.i].update = DATA_DEPRECATED;
			return &DATA_LIST[Map.i];
		}
	}
	return 0;
}

// CANP Interface
void MAP_setDataFromCANP(CANP_Data *data, uint32_t timestamp) {
	MAP_setData(data->header.ID, data->payload, timestamp, data->header.status);
}

uint32_t MAP_getDataToCANP(DATA_ID key, CANP_Data *data) {
	Map.currentData = MAP_getData(key);
	data->header.ID = Map.currentData->key;
	data->payload = Map.currentData->value;
	data->header.status = Map.currentData->update;
	return Map.currentData->timestamp;
}

// Von J
void set_to_data_list(DATA_ID key, DATA_FIELD *pList, uint32_t new_value) {
	MAP_setData(key, new_value, 333, 1);
}

void construct_data_list(DATA_FIELD *pList) {
	DATA_FIELD test;
	test.key = FSMS_SPEED;
	DATA_LIST[0] = test;
	test.key = RSMS_PRESSURE_1;
	DATA_LIST[1] = test;
}

uint32_t get_from_data_list(DATA_ID key, DATA_FIELD *pList) {
	DATA_FIELD *data = MAP_getData(key);
	return data->value;
}

// Init
void MAP_initDataField(DATA_ID key) {
	DATA_LIST[Map.i].key = key;
	DATA_LIST[Map.i].value = 0;
	DATA_LIST[Map.i].update = DATA_NOT_AVAILABLE;
	DATA_LIST[Map.i].timestamp = 0;

	Map.i++;
}

///*
void MAP_init() {
	Map.i = 0;

	MAP_initDataField(SYSTEM_NULL_DATA);
	MAP_initDataField(SYSTEM_STATUS);
	MAP_initDataField(MCU);
	MAP_initDataField(VAS_NULL_DATA);
	MAP_initDataField(VAS_STATUS);
	MAP_initDataField(VAS_GOPRO_1);
	MAP_initDataField(VAS_GOPRO_2);
	MAP_initDataField(VAS_GOPRO_3);
	MAP_initDataField(VAS_GOPRO_4);
	MAP_initDataField(VAS_GOPRO_5);
	MAP_initDataField(VAS_PICAM);
	MAP_initDataField(RSMS_NULL_DATA);
	MAP_initDataField(RSMS_STATUS);
	MAP_initDataField(RSMS_PRESSURE_1);
	MAP_initDataField(RSMS_PRESSURE_2);
	MAP_initDataField(RSMS_PRESSURE_7);
	MAP_initDataField(RSMS_PRESSURE_8);
	MAP_initDataField(RSMS_PT_1);
	MAP_initDataField(RSMS_PT_2);
	MAP_initDataField(RSMS_PT_7);
	MAP_initDataField(RSMS_PT_8);
	MAP_initDataField(FSMS_NULL_DATA);
	MAP_initDataField(FSMS_STATUS);
	MAP_initDataField(FSMS_ACC1_X);
	MAP_initDataField(FSMS_ACC1_Y);
	MAP_initDataField(FSMS_ACC1_Z);
	MAP_initDataField(FSMS_GYR1_X);
	MAP_initDataField(FSMS_GYR1_Y);
	MAP_initDataField(FSMS_GYR1_Z);
	MAP_initDataField(FSMS_ACC2_X);
	MAP_initDataField(FSMS_ACC2_Y);
	MAP_initDataField(FSMS_ACC2_Z);
	MAP_initDataField(FSMS_GYR2_X);
	MAP_initDataField(FSMS_GYR2_Y);
	MAP_initDataField(FSMS_GYR2_Z);
	MAP_initDataField(FSMS_LAT);
	MAP_initDataField(FSMS_LONG);
	MAP_initDataField(FSMS_HEIGHT);
	MAP_initDataField(FSMS_SPEED);
	MAP_initDataField(FSMS_HEADING);
	MAP_initDataField(FSMS_ROC);
	MAP_initDataField(FSMS_GPS_STATUS);
	MAP_initDataField(FSMS_GPS_NSAT);
	MAP_initDataField(FSMS_GPS_GDOP);
	MAP_initDataField(PCDU_NULL_DATA);
	MAP_initDataField(PCDU_STATUS);
	MAP_initDataField(IFC_NULL_DATA);
	MAP_initDataField(IFC_STATUS);
	MAP_initDataField(IFC_DOWNLINK);
	MAP_initDataField(GSE_NULL_DATA);
	MAP_initDataField(GSE_STATUS);
	MAP_initDataField(MAX_DATA_ID);
}
