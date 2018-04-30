/**\file
 * dac.h
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */
#include "spi.h"
#include <stdint.h>
#ifndef DAC_H_
#define DAC_H_

#define GA BITD                 //setting gain to 1
#define SHDN BITC               //not shutting down
#define BUF 0                   //disabling buffer
#define DAC_MAX_VAL 4095
#define DAC_MIN_VAL 0
#define DAC_2V 2482

/** \brief This function takes in a 12-bit encoded voltage and sends sends a formatted
 * message over SPI
 *
 *  This function takes in a 12-bit number representing a voltage from 0 to 3.3V.
 *  The function mapping the 12-bit number to voltage is as follows:
 *  \f$ V_{out} = \frac{(V_{ref}\times D_n)}{2^n}*G\f$
 *  Where:
 *  \f$G=1\f$ and
 *  \f$V_{ref}=3.3V\f$
 *
 *
 *  \param voltage A 12-bit number encoded encoding a voltage
 *  \return void
 *
 */
void dacOut(uint16_t voltage);
uint16_t voltageOut(float desiredV);
#endif /* DAC_H_ */
