/** \file combo.h
 * \brief Function prototypes for the combo lock
 *
 * This file contains all of the headers and function prototypes required.
 * These files are more of helper functions
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 *
 */

#ifndef COMBO_H_
#define COMBO_H_

#include "lcd.h"
#include "keypad.h"

/** \brief Maximum size allowed for the combo lock
 *
 * Can succesfully be added to 6 without major issues... theoretically
 */
#define COMBO_SIZE 4

/** @brief Displays a locked screen
 *
 * This function will display on the LCD
 * a set of text that indicates that the screen is locked.\n
 * The first row will display "Locked"\n
 * The second row will display "Enter Key"\n
 * The cursor will be left one space after "y"
 *
 * \return void
 */
void displayLockedScreen(void);

/** \brief Displays the unlocked screen
 *
 * This function will display text to show the user that the lock is not unlocked.\n
 * The first row will say "Hello World"\n
 * The second row will say "New Key: "\n
 *
 * \return void
 */
void displayUnlockedScreen(void);

/** \brief Checks checkKP() result to see if Asterisk has been pressed
 *
 * This function takes in the row/column encoded data of checkKP() and returns
 * a boolean. If the pressed key is asterisk, then the function returns a 1
 * and will display the locked screen. Else, the function will return 0 and
 * leave the screen alone
 *
 * \return 1 if input is equivalent to asterisk, 0 otherwise
 * \param data the row/col encoded data resulting from checkKP()
 */
uint32_t checkAsterisk(uint32_t data);

/** \brief checks to see if the input string matches the output string
 *
 * This function takes in pointers of a string input and a key. If the key and
 * the string match, then the function will return true. The function assumes
 * that both strings are of comboSize long.
 *
 * \return 1 If input matches key, 0 otherwise
 * \param input A pointer to the input char array
 * \param key A pointer to the second char array
 * \bug the function will get a error out if one of the char arrays isn't
 * initialized to the correct size. This is because the index will try to read
 * a data element outside of allocated memory
 *
 */
uint8_t checkCode(uint8_t* input, uint8_t* key);

#endif /* COMBO_H_ */
