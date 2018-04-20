/*
 * delay.h
 *
 *  Created on: Apr 4, 2018
 *      Author: Jason Zhou
 */

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#include "stdint.h"

#define KHZ (uint32_t)1000 //Multiplying frequency in kHz by 1000 for MHz
#define US (uint32_t)1000000 //Converting seconds to us

void delay_ms(uint32_t ms, uint32_t freq);
void delay_us(uint32_t us, uint32_t freq);


#endif /* DELAY_H_ */
