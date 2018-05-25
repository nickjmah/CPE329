/*
 * hx711.h
 *
 *  Created on: May 24, 2018
 *      Author: Jason Zhou
 */

#ifndef HX711_H_
#define HX711_H_

#include "msp.h"

#define HX711_STRUCT P1 //to be changed once hardware is finalized
#define HX711_CLK 0     //""
#define HX711_DO 1      //""

void initHX711(void);
void powerDown(void);
uint32_t readCount(void);


#endif /* HX711_H_ */
