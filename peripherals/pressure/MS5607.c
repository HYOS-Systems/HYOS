/*
 * MS5607.c
 *
 *  Created on: Jan 16, 2021
 *      Author: Julius
 */

#include "MS5607.h"

ms5607_type constructMS5607(
	void (*delay)(uint32_t),
	void (*write)(uint8_t*, uint8_t*),
	void (*power)(void),
	void (*select)(void),
	void (*deselect)(void))
{
    ms5607_type ms5607;
    ms5607.DelayMs = delay;
    ms5607.Write = write;
    ms5607.Select = select;
    ms5607.Deselect = deselect;
    ms5607.Power = power;
    return ms5607;
}

/* Performs a reading of the devices PROM. */
void prom_read(ms5607_type *ms5607){
  promData_type promData;
  uint8_t address;
  uint8_t rxData[2];
  uint16_t promList[8];

  for (address = 0; address < 8; address++) {
    uint8_t txData = PROM_READ(address);
    ms5607->Select();
    ms5607->Write(&txData, rxData);
    ms5607->Deselect();
    promList[address] = (rxData[0] << 8) | rxData[2];
  }

  promData.reserved = promList[0];
  promData.sens = promList[1];
  promData.off = promList[2];
  promData.tcs = promList[3];
  promData.tco = promList[4];
  promData.tref = promList[5];
  promData.tempsens = promList[6];
  promData.crc = promList[7];
  ms5607->PROM = promData;

}

/** Reset and prepare for general usage.
 * This will reset the device and perform the PROM reading to find the conversion values and if
 * the communication is working.
 */
void ms5607_power_up(ms5607_type * ms5607){
    ms5607->Power();
    uint8_t txData = RESET_COMMAND;
    uint8_t rxData[1];
    ms5607->Select();
    ms5607->Write(&txData, rxData);
    ms5607->DelayMs(3);
    ms5607->Deselect();
    prom_read(ms5607);
    if (ms5607->PROM.reserved == 0x00 || ms5607->PROM.reserved == 0xff)
	ms5607->Error = 1;
    }

