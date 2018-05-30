/*
 * scale.h
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */
#include "lcd.h"
#ifndef SCALE_H_
#define SCALE_H_


/**sets up the LCD initial display
 *
 */

void initScale(void);

/** \brief promps the user for their height and updates a variable that holds it
 *
 */
void updateHeight(void);

#endif /* SCALE_H_ */
