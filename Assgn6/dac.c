/*
 * dac.c
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */
#include "dac.h"
void dacOut(uint16_t voltage)
{
    uint8_t bytePacket[2] = {};
    uint16_t dataByte = voltage | GA | SHDN | BUF;
    bytePacket[0] = (dataByte>>8);
    bytePacket[1] = dataByte & (0x00ff);
    sendData(bytePacket, sizeof(bytePacket));
    return;
}



