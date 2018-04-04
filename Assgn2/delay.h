/*
 * delay.h
 *
 *  Created on: Apr 4, 2018
 *      Author: Nick
 */

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED
#include "stdint.h"

#define KHZ_IN_HZ 1000
#define MS_IN_S 1000
#define US_IN_S 1000000

int delay_ms(int ms, uint16_t freq);

int delay_us(int us, uint16_t freq);

#endif /* DELAY_H_ */
