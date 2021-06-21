
#pragma once

#include "systems/compass/ifc/ifc_types.h"

void IFC_DH_init(IFC_PeripheralStruct *);
void IFC_DH_canHandle(CANBus*);
void IFC_DH_sendTransition(STATE_ID);
