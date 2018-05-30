/*
 * scale.c
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */

#include "scale.h"


static Measurement currentMeas = {
                                  Unit{"00",22},//TODO:Figure out what it should be
                                  0, //height
                                  0, //weight
                                  0};//bmi
}
void initScale(void)
{
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
    currentMeas.height = 12*2;
}
