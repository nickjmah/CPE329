/*
 * adc.h
 *
 *  Created on: May 10, 2018
 *      Author: Jason Zhou
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

#define ADC_STRUCT P5
#define ADC_0 BIT4
#define ADC_CAL 0

void initADC(void);
uint16_t readADCFlag(void);
uint32_t readADC(void);
void ADC14_IRQHandler(void);

#endif /* ADC_H_ */
