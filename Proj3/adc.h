/*
 * adc.h
 *
 *  Created on: May 10, 2018
 *      Author: Jason Zhou
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
#include "timer.h"

#define ADC_STRUCT P5
#define ADC_0 BIT4
#define ADC_CAL 0

uint8_t* itoa(int val);
void initADC(void);
void startConv(void);
uint16_t readADCFlag(void);
uint32_t readADC(void);
uint32_t averageADC(uint32_t* arrayADC, uint8_t arraySize);
uint32_t averageDC(void);
void ADC14_IRQHandler(void);
//void TA0_0_IRQHandler(void);

#endif /* ADC_H_ */
