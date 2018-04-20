/**
 * @file combo.h
 *
 *  Created on: Apr 17, 2018
 *      Author: Nick
 */

#ifndef COMBO_H_
#define COMBO_H_

#include "lcd.h"
#include "keypad.h"
#define COMBO_SIZE 4
/**
 * @brief displays a locked screen
 *
 * This function will display on the LCD
 * a set of text that indicates that the screen is locked.
 * The first row will display "Locked"
 * The second row will display "Enter Key"
 * The cursor will be left one space after "y"
 */
void displayLockedScreen(void);
void displayUnlockedScreen(void);
uint32_t checkAsterisk(uint32_t data);
uint8_t checkCode(uint8_t* input, uint8_t* key);


#endif /* COMBO_H_ */
