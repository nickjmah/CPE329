/*
 * scale.c
 *
 *  Created on: May 30, 2018
 *      Author: Nick
 */

#include "scale.h"

Unit pounds = { lb, "lb", 2.2 };
Unit kilograms = { kg, "kg", 1 };

static Measurement currentMeas = { { lb, "lb", 2.2 }, 60, //height
                                   71, //weight
                                   0 }; //bmi
void initScale(void)
{
    //printing weight
    writeString("Weight:  0 ");
    rowShiftDown();
    writeString("BMI: 0");
}

void updateHeightFt(void)
{
    returnHome();
    clearDisplay(); //TODO: Figure out how to only clear the bottom row
    writeString("Height('):");
}
void updateHeightIn(void)
{
    returnHome();
    clearDisplay();
    writeString("Height(\")");
}
void changeHeight(uint32_t height)
{
    currentMeas.height = height;
    return;
}
void updateUnits(void)
{
    returnHome();
    clearDisplay();
    writeString("Units:");
    rowShiftDown();
    writeString("1: SI, 2; Imp");
}
void updateScale(void)
{
    static char buf[50];
    updateWeight(10);
    calcBMI();
    returnHome();
    clearDisplay();
    writeString("Wt(");
    writeString(currentMeas.unit.name);
    writeString("): ");
    sprintf(buf, "%.2f",currentMeas.weight * currentMeas.unit.scale);
    writeString(buf);
    rowShiftDown();
    writeString("BMI:");
    writeString(itoa(currentMeas.bmi));
}
void updateSI(void)
{
    currentMeas.unit = kilograms;
}
void updateImp(void)
{
    currentMeas.unit = pounds;
}
void calcBMI(void)
{
    currentMeas.bmi = currentMeas.weight
            / (currentMeas.height * currentMeas.height * 0.0254 * 0.0254); //checkfor float
}
void updateWeight(uint8_t numAvg)
{
    currentMeas.weight = getUnits(numAvg);
}
void calScreen(void)
{
    returnHome();
    clearDisplay();
    writeString("Cal Wt(Kg):");
}

