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



typedef enum unitName {kg,lb}unitName_t;
/** \brief struct for convenience to aggregate the scale factor and name
 *
 */
typedef struct Unit{
    unitName_t unitName;
    char name[2];//size of the name, must be 2 letters, ex. "lb","kg"
    float scale; //some divisor to turn into the correct units
}Unit;
/** \brief initializes ports for serial communication with HX711
 *  Sets up data input and clock output
 */
typedef struct Measurement{
    Unit unit;          //For unit conversion
    uint32_t height;    //height in inches
    float weight;    //weight in kg
    float bmi;       //Body Mass Index
}Measurement;

/**sets up the LCD initial display
 *
 */
void initScale(void);

/** \brief prompts the user for their height and updates a variable that holds it
 *
 */
void changeHeight(uint32_t height);
void updateUnits(void);
void updateSI(void);
void updateImp(void);
void updateScale(void);
void updateWeight(uint8_t numAvg);
void calcBMI(void);
char* itoa(int val);
void updateHeightFt(void);
void updateHeightIn(void);
void changeHeight(uint32_t height);
void calScreen(void);
#endif /* SCALE_H_ */
