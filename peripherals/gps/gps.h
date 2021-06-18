/*
 * gps.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Julius
 */

#include "drivers/hyend_os_drivers.h"

typedef struct{
    char get_msg[4];
    char f00_message[121];
    char f48_message[44];
    char f62_message[597];
    uint8_t current_msg;
} phoenix_type;
