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
void displayUnlockedScreen(void);
{
    returnHome();
    writeString("Hello World");

}


