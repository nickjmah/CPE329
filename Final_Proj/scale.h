/*
 * scale.h
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */
#include "lcd.h"
#ifndef SCALE_H_
#define SCALE_H_

/** \brief struct for convenience to aggregate the scale factor and name
 *
 */
typedef struct Unit{
    char name[2];//size of the name, must be 2 letters, ex. "lb","kg"
    float scale; //some divisor to turn into the correct units
}Unit;
/**sets up the LCD initial display
 *
 */

void initScale(void);

/** \brief changes the units used for printing and math
 *
 */
void changeUnit(Unit choice);

/** \brief promps the user for their height and updates a variable that holds it
 *
 */
void updateHeight(void);

#endif /* SCALE_H_ */
