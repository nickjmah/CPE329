/*
 * scale.c
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */

#include "scale.h"


static Measurement currentMeas = {
                                  {kg,"00",22},
                                  0, //height
                                  0, //weight
                                  0};//bmi
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
    clearDisplay(); //TODO: Figure out how to only clear the bottom row
    writeString("Height('):");
    currentMeas.height = 12*2;//TODO:Figure out how to update this value
    writeString("Height('):")
    currentMeas.height += 1;
}
void updateUnits(void)
{
    returnHome();
    clearDisplay();
    writeString("Units:")
    rowShiftDown();
    writeString("1: SI, 2; Imp");
}
void updateScale(void)
{
    returnHome();
    clearDisplay();
    writeString("Wt("));
    writeString(currentMeas.unit.name);
    writeString(")");
    writeString(atoi(currentMeas.weight*currentMeas.unit.scale));//TODO:make sure that this is correct
    rowShiftDown();
    writeString("BMI:")
    writeString(atoi(currentMeas.bmi));
    snarkyMeas();
}
void snarkyMeas(void)
{
    writeString("F A T");
}
void calcBMI(void)
{
    if (currentMeas.unit.unitName==kg);
}

