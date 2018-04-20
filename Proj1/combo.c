/**
 * combo.c
 * @file combo.h
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
    rowShiftDown();
    writeString("New Key: ");
}

uint8_t checkCode(uint8_t* input, uint8_t* key)
{
    int i = 0;
    while(i<COMBO_SIZE)
    {
        if(*input != key[i])
            return 0;
        input++;
        i++;
    }
    return 1;
}

