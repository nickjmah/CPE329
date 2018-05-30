/*
 * scale.c
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */

#include "scale.h"


uint32_t height=44444;
void initScale(void)
{
    currentUnit = pounds;
    //printing weight
//    writeString("Weight: 0" + currentUnit.name);
    rowShiftDown();
    writeString("BMI: 0");
}

void updateHeight(void)
{
    returnHome();
    clearDisplay();
    writeString("Height('):");


}
