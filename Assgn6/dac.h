/*
 * dac.h
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */
#include "spi.h"
#include <stdint.h>
#ifndef DAC_H_
#define DAC_H_

#define GA BIT5                 //setting gain to 1
#define SHDN BIT4               //not shutting down
#define BUF 0                   //disabling buffer

/** \brief This function takes in a 12-bit encoded voltage and sends sends a formatted
 * message over SPI
 *
 *  This function takes in a 12-bit number representing a voltage from 0 to 3.3V.
 *  The function mapping the 12-bit number to voltage is as follows:
 *
 *
 *  \param voltage A 12-bit number encoded encoding a voltage
 *  \return void
 *
 */
void dacOut(uint16_t voltage);

#endif /* DAC_H_ */
