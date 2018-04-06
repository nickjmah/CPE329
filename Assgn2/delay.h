/*
 * delay.h
 *
 *  Created on: Apr 4, 2018
 *      Author: Nick
 */

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED
#include "stdint.h"

#define KHZ_IN_HZ (uint32_t)1000
#define MS_IN_S (uint32_t)1000
#define US_IN_S (uint32_t)1000000
#define CYC_PER_LOOP (uint32_t)6
#define CYC_FOR_LED (uint32_t)14
int delay_ms(int ms, uint32_t freq);

int delay_us(int us, uint32_t freq);

#endif /* DELAY_H_ */
