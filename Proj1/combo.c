/** \file combo.c
 * \brief Helper functions to enable the combo lock
 *
 * This file contains helper functions to control the operation of the display
 * and check for specific key presses. This code could be moved into main
 * without much difficulty
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
#include "combo.h"
void displayLockedScreen(void)
{
    clearDisplay();             //clear the display
    returnHome();               //set the cursor to the origin
    writeString("Locked");      //write "Locked on the first row
    rowShiftDown();             //move to the second row
    writeString("Enter Key ");  //write "Enter Key" on the second row
}

uint32_t checkAsterisk(uint32_t data)
{
    if (data == STAR){          //check if the encoded data matched asterisk
        clearDisplay();         //clear the display if true
        displayLockedScreen();  //display the locked screen
        return 1;               //send verification that the key pressed is
                                //asterisk
    }
    return 0;                   //send false otherwise
}

void displayUnlockedScreen(void)
{
    clearDisplay();             //clear the display
    returnHome();               //send cursor to the origin
    writeString("Hello World");
    rowShiftDown();
    writeString("New Key: ");
}

uint8_t checkCode(uint8_t* input, uint8_t* key)
{
    int i = 0;                  //create an iterator
    while(i<COMBO_SIZE)         //iterate only until the size of the combo
    {
        if(*input != key[i])    //dereference the pointer and compare it to
                                //the ith element of key
            return 0;
        input++;                //increment the pointer address and the iterator
        i++;
    }
    return 1;
}

