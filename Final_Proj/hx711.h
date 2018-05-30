/*
 * hx711.h
 *
 *  Created on: May 24, 2018
 *      Author: Jason Zhou
 */

#ifndef HX711_H_
#define HX711_H_

#include "msp.h"

#define HX711_STRUCT P5 //to be changed once hardware is finalized
#define HX711_CLK BIT4     //""
#define HX711_DO BIT5      //""

void initHX711(void);
void powerUp(void);
void powerDown(void);
uint32_t readCount(void);
uint32_t readAverage(uint8_t times);
uint32_t getValue(uint8_t times);
float getUnits(uint8_t times);
void tare(uint8_t times);
void setScale(float newScale);
float getScale(void);
void setOffset(uint32_t newOffset);
uint32_t getOffset(void);



#endif /* HX711_H_ */
