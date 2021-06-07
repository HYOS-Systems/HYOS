/*
 * canProtocol.h
 *
 *  Created on: 05.04.2021
 *      Author: Bayram
 */

#pragma once

#include "peripherals/hyend_os_peripherals.h"
#include "hyendOS/CANProtocol/App/canProtocol.h"
#include <hyendOS/StateTransition/stateTranstion.h>

#ifdef INC_CANTRI_H_
#define CANPROTOCOLIMPL_H_
#endif

MESSAGE_TYPE CANI_receiveMessage(CANBus*, MessageHeader*);
uint8_t CANI_isThisTarget(MessageHeader*);

void CANI_sendData(CANBus*, DataMessage*);
void CANI_sendState(CANBus*, StateMessage*);
void CANI_sendRequestData(CANBus*, RequestDataMessage*);
void CANI_sendRequestState(CANBus*, StateMessage*);

void CANI_interpretDataMessage(CANBus*, DataMessage*);
void CANI_interpretStateMessage(CANBus*, StateMessage*);
void CANI_interpretRequestDataMessage(CANBus*, RequestDataMessage*);
void CANI_interpretRequestStateMessage(CANBus*, StateMessage*);
void CANI_interpretTransitionMessage(CANBus*, TransitionMessage*);
