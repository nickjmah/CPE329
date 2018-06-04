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

#define ITOA_BUF_SIZE 32        //size of the ITOA buffer
#define ITOA_INDEX_START 30

typedef struct Measurement{
    Unit unit;          //For unit conversion
    uint32_t height;    //height in inches
    uint32_t weight;    //weight in unit
    uint32_t bmi;       //Body Mass Index
}Measurement;


/**sets up the LCD initial display
 *
 */
void initScale(void);

/** \brief prompts the user for their height and updates a variable that holds it
 *
 */
void updateHeight(void);
void updateUnits(void);
void updateScale(void);
void calcBMI(void);
void snarkyMeas(void);
char* itoa(int val);
#endif /* SCALE_H_ */
