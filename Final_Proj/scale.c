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
    writeString("Height('):");
    currentMeas.height += 1;
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
    returnHome();
    clearDisplay();
    writeString("Wt(");
    writeString(currentMeas.unit.name);
    writeString(")");
    writeString(itoa(currentMeas.weight*currentMeas.unit.scale));//TODO:make sure that this is correct
    rowShiftDown();
    writeString("BMI:");
    writeString(itoa(currentMeas.bmi));
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
char* itoa(int val)
{
    static char buf[ITOA_BUF_SIZE] = { 0 }; //create a static buffer for string output
    int i = ITOA_INDEX_START;
    buf[i + 1] = '\0';              //forcing termination character in the event
                                    //of unexpected behaviour
    for (; val != 0; i--)                  //loop until val is 0
    {
        buf[i] = "0123456789"[(val) % 10]; //give buf the least significant bit of val
        val /= 10;                        //decimal shift right val
    }
    return &buf[i + 1];                    //return index of beginning of string
}

