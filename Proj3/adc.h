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
#define ADC_CAL 0               //DC offset calibration for ADC
#define ADC_MAX_VAL 16384       //maximum decimal value of the ADC
#define ADC_MAX_ANALOG_VAL 3300 //maximum voltage for the ADC in mV
#define UNITS_MILLI 1000
#define ITOA_BUF_SIZE 32        //size of the ITOA buffer
#define ITOA_INDEX_START 30

/** \brief converts a decimal number into a string
 *  itoa takes in an unsigned integer and converts it into a string representation.
 *  A static buffer holds the result of the number. The function works like a decimal
 *  shift register. Val is continuously decimal shifted right and the least significant digit
 *  is used as the index of a string for the next value of buf. A pointer to the first
 *  value of buffer is returned and can be used in generic string print function
 *
 *  \param val A number to be turned into a string
 *  \return pointer to the buffer holding the first character of the string
 */
char* itoa(int val);
/** \brief initializes the ADC
 *  Sets up P5.4 as a 14-bit ADC pin with interrupts. The ADC is run in sample and hold
 *  mode with multiple sample and conversion. The ADC will be taking a Vref of 3.3V as well
 *
 *  \return void
 */
void initADC(void);
/** \brief Sends a command to initialize a conversion
 *
 */
void startConv(void);
/** \brief reads the local ADC conversion complete flag
 *
 */
uint16_t readADCFlag(void);
/** \brief reads the value of the ADC stored as a file local global variable
 *
 */
uint32_t readADC(void);
/** \brief IRQHandler for the AD
 *
 *  This IRQ is called when the measurement is ready. Upon execution, the value will be
 *  saved to a global variable for later usage and a flag will be rased
 *
 */
void ADC14_IRQHandler(void);
//void TA0_0_IRQHandler(void);

#endif /* ADC_H_ */
