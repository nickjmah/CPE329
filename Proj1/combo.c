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


