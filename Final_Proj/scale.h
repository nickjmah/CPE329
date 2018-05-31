/*
 * scale.h
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */
#include "lcd.h"
#include "hx711.h"
#ifndef SCALE_H_
#define SCALE_H_

typedef struct Measurement{
    Unit unit;
    uint32_t height;    //height in inches
    uint32_t weight;    //weight in unit
    uint32_t bmi;       //Body Mass Index
}Measurement;


/**sets up the LCD initial display
 *
 */
void initScale(void);

/** \brief promps the user for their height and updates a variable that holds it
 *
 */
void updateHeight(void);
void calcBMI(void);
#endif /* SCALE_H_ */
