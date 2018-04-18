/*
 * combo.c
 *
 *  Created on: Apr 17, 2018
 *      Author: Nick
 */

void displayLockedScreen(void)
{
    returnHome();
    writeString("Locked");
    rowShiftDown();
    writeString("Enter Key")
}
void displayUnlockedScreen(void);
{
    returnHome();
    writeString("Hello World");

}


