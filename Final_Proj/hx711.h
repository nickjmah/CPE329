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
/** \brief lowers clock output which powers up the HX711
 *
 */
void powerUp(void);
/** \brief raises clock output which powers down the HX711
 *
 */
void powerDown(void);
/** \brief reads in clocked bits from the HX711
 *
 */
int32_t readCount(void);
/** \brief calls readCount and averages over a number of times
 *
 */
int32_t readAverage(uint8_t times);
/** \brief returns readAverage with the offset subtracted
 *
 */
int32_t getValue(uint8_t times);
/** \brief returns getValue divided by the scale factor
 *
 */
float getUnits(uint8_t times);
/** \brief sets the offset by calling readAverage
 *   Meant to be called with no object on the scale
 *   The averaged value is then set as the offset, which gets subtracted
 */
void tare(uint8_t times);
/** \brief sets the scale factor
 *
 */
void setScale(float newScale);
/** \brief returns the scale factor
 *
 */
float getScale(void);
/** \brief sets the offset value
 *
 */

void setOffset(int32_t newOffset);
/** \brief returns the offset value
 *
 */
int32_t getOffset(void);
/** \brief calibrates the scale value using a known weight
 *
 */
void calibrate(float weight);

#endif /* HX711_H_ */
