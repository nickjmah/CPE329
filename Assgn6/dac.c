/** \file dac.c
 * \brief DAC data processing and transmission
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
#include "dac.h"
void dacOut(uint16_t voltage)
{
    uint8_t bytePacket[2] = {}; //initializing uint8_t array carrying two bytes of data
    uint16_t dataByte = voltage | GA | SHDN | BUF; //masking voltage input value with output
                                                   //gain, shutdown, and buffer bits
    bytePacket[0] = (dataByte>>8); //setting first entry of array as the upper byte
    bytePacket[1] = dataByte & (0x00ff); //setting second entry as the lower byte
    sendData(bytePacket, sizeof(bytePacket)); //sending data array through SPI command
    return;
}

uint16_t voltageOut(float desiredV)
{
    return desiredV * 4095 / 3.3; //converting floating point voltage to DAC input bits
}


