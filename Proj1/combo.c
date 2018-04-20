/*
 * combo.c
 *
 *  Created on: Apr 17, 2018
 *      Author: Nick
 */
/**
 * This is a test
 */
#include "combo.h"
void displayLockedScreen(void)
{
    clearDisplay();
    returnHome();
    writeString("Locked");
    rowShiftDown();
    writeString("Enter Key ");
}

uint32_t checkAsterisk(uint32_t data){
    if (data == STAR){
        clearDisplay();
        displayLockedScreen();
        return 1;
    }
    return 0;
}

void displayUnlockedScreen(void)
{
    clearDisplay();
    returnHome();
    writeString("Hello World");

}

uint8_t checkCode(uint8_t* code)
{
    int i = 0;
    while(i<3)
    {
        if(*code != WOMBO_COMBO[i])
            return 0;
        code++;
        i++;
    }
    return 1;
}

