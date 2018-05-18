/*
 * dmm.h
 *
 *  Created on: May 16, 2018
 *      Author: Jason Zhou
 */

#ifndef DMM_H_
#define DMM_H_

#include <stdint.h>
#include <stdio.h>
#include "timer.h"
#include "delay.h"
#include "freq.h"
#include "adc.h"

#define DMM_STRUCT P2
#define DMM_0 BIT4

void initFreqMeas(void);
uint16_t readFreqFlag(void);
uint16_t readRisingFlag(void);
void clearRisingFlag(void);
uint32_t readPeriod(void);
uint32_t calcFreq(void);
uint32_t averageDC(void);
uint32_t averageOffset(void);
uint32_t * peakToPeak(void);
uint32_t PTPCalc(uint32_t* array);
void clearDoneFlag(void);
void TA0_N_IRQHandler(void);



#endif /* DMM_H_ */
