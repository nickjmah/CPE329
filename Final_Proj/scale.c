/*
 * scale.c
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */

#include "scale.h"


Unit pounds = {"lb", 202};
Unit kilograms = {"kg", 232};
Unit currentUnit;
uint32_t height=44444;
void initScale(void)
{
    currentUnit = pounds;
    //printing weight
    writeString("Weight: 0" + currentUnit.name);
    rowShiftDown();
    writeString("BMI: 0");
}

void changeUnit(Unit choice)
{
    currentUnit = choice;
}
void updateHeight(void)
{
    returnHome();
    clearDisplay();
    writeString("Height('):");


}
