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
#define WAVEFORM_ID_CUTOFF 8700

/** \brief initializes timers for frequency measurement
 *  Enables CCR[0] for a 100us interrupt and sets the timer to take
 *  48MHz in continouous mode. Also, sets up a capture compare flag
 *  on P2.4 with rising edge detection and interruot.
 *  Finally, adding an overflow interrupt for timer_a0
 */
void initFreqMeas(void);
/** \brief returns the frequency measurement complete flag
 *
 */
uint16_t readFreqFlag(void);
/** \brief reads the cycle between the two capture flags
 *
 */
uint32_t readPeriod(void);
/** \brief Calculates the amount of cycles that have occured.
 *
 */
uint32_t calcFreq(void);
/** calculates the average DC Value
 * DCMeas is clocked on a 100us non-interrupt delay. At the start of
 * each loop, we set the blocking timer to operate for 100us from the
 * start. We accumulate each value. AFter 1ms has passed, we return
 * the average value.
 * \return The average DC val in terms of ADC output
 */
uint32_t averageDC(void);
/** Calculates the pea-peak through the min and max difference
 *
 */
uint32_t PTPCalc(uint32_t minVal, uint32_t maxVal);
/** CAlculates the offset by calculated the median of max ansm in_val
 *
 */
uint32_t OffsetCalc(uint32_t minVal, uint32_t maxVal);
/** \brief Takes all AC Measurements
 *   ACMeas works on a loop clocked at 100us that lasts for 1 second.
 *   Each loop, we measure the AD and manipulate a group of variables.
 *   We check and replace the stored min and max values if applicable.
 *   Then we square and accumulate all of the values for an rms calc.
 *   After 1 second has passed, we return the average of the
 *   accumulated squares as well as the min and max value.
 *   \param ACVals a pointer to the array of numbers that get modified
 *   by ACMeas
 */
void ACMeas(uint32_t* ACVals);
/** \brief takes in the RMS, peak-peak and offset voltage and outputs
 *         A string result of the answer
 *
 *   This function takes in all the parameters needed to calculate
 *   the RMS voltage of a sine and triangle wave. If the peak-peak
 *   voltage is smaller than we can reliably differentiate the
 *   triangle and sine rms values, we output "INPUT TOO LOW" to
 *   indicate that the waveform is not distinguishable. If the rms we
 *   measure is within the threshold of a sine wave, we output a
 *   "SINE". IF it matches a triangle wave, it outputs
 *   a "TRIANGLE. Otherwise, we output "SQUARE".
 *   \param RMS The measured RMS value
 *   \param PTP The measured peak to peak voltage
 *   \param OFS The measured AC OFfset voltage
 *   \return a pointer to a string to output to the terminal
 */
char* waveDetect(uint32_t RMS, uint32_t PTP, uint32_t OFS);
/** \brief IRQ for frequency measurements
 *
 *  This function has two interrupts serviced: timeroverflow and
 *  capture rising edge. The timer overflow handler simply increments
 *  overflow whenever called. The rising edge capture starts by
 *  storing the value into a buffer based on captureCount. Afterwards
 *  captureCount is incremented and we check if this is the beginning
 *  or end of our frequency measurement based on whether captureCount
 *  is at the first or second index of our buffer. If it is the first,
 *  we only clear the overflow flag. On the second capture, we calculate
 *  the period using overflow and the two capture values. Then we raise
 *  a flag for a non interrupt function to service it.
 */
void TA0_N_IRQHandler(void);

#endif /* DMM_H_ */
