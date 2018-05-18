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
#include "freq.h"
#include "adc.h"
#include "math.h"
#include "uart.h"

#define DMM_STRUCT P2
#define DMM_0 BIT4
#define THRES 120 //threshold for determining waveform type

/** \brief initializes timers for frequency measurement
 *  Enabes a
 */
void initFreqMeas(void);
uint16_t readFreqFlag(void);
uint32_t readPeriod(void);
uint32_t calcFreq(void);
uint32_t averageDC(void);
uint32_t PTPCalc(uint32_t minVal, uint32_t maxVal);
uint32_t OffsetCalc(uint32_t minVal, uint32_t maxVal);
void ACMeas(uint32_t* ACVals);
char * waveDetect(uint32_t RMS, uint32_t PTP, uint32_t OFS);
void TA0_N_IRQHandler(void);



#endif /* DMM_H_ */
