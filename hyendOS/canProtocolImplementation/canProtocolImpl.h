/*
 * canProtocol.h
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#pragma once

#include "connectivity/canTri/canTri.h"
#include "core/hyend_os_core.h"

#ifdef INC_CANTRI_H_
#define CANPROTOCOLIMPL_H_
#endif

MESSAGE_TYPE CANI_receiveMessage(CANBus*, CANP_MessageHeader*);
uint8_t CANI_isThisTarget(CANP_MessageHeader*);

void CANI_sendData(CANBus*, CANP_DataMessage*);
void CANI_sendRequestData(CANBus*, CANP_RequestDataMessage*);
void CANI_sendTransition(CANBus*, CANP_TransitionMessage*);

void CANI_interpretDataMessage(CANBus*, CANP_DataMessage*);
void CANI_interpretRequestDataMessage(CANBus*, CANP_RequestDataMessage*);
void CANI_interpretTransitionMessage(CANBus*, CANP_TransitionMessage*);
