/*
 * scale.h
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */
#include "lcd.h"
#include "hx711.h"
#include "stdio.h"
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
/** prints to lcd the units prompt */
void updateUnits(void);
/**
 * changes the units to SI
 */
void updateSI(void);
/**
 * changes the units to Imperial
 */
void updateImp(void);
/**
 * Measures the scale and updates weight and BMI:
 * This function calls updateWeight() with an average of 10,
 * scales the value based off of the desired output unit and
 * prints a formatted float through sprintf. BMI is also printed to
 * the screen using a custom itoa function that converts the BMI to an
 * int
 */
void updateScale(void);
/**
 * updates the value of the Measurement struct by calling a hx711.h function
 * @param numAvg Number of averages to be performed
 */
void updateWeight(uint8_t numAvg);
/**
 * calculates the BMI using the formula: wt(kg)/(height(m))^2
 */
void calcBMI(void);
/**
 * displays the prompt for entering the feet portion of your height
 */
void updateHeightFt(void);
/** displays the prompt for entering the inches portion of your height */
void updateHeightIn(void);
/**
 * updates the height parameter of the static struct
 * @param height The height to update the measurement struct with
 */
void changeHeight(uint32_t height);
/**
 * displays the calibration screen
 */
void calScreen(void);
#endif /* SCALE_H_ */
