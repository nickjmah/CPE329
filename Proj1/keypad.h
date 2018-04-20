/** @file keypad.h
 * @brief Function prototypes for the keypad
 *
 * This file contains all of the headers and function prototypes required for
 * the keypad to operate properly.
 *
 * @author Nick Mah
 * @author Jason Zhou
 *
 *
 */
#include "msp.h"



#ifndef KEYPAD_H_
#define KEYPAD_H_


#define COL_STRUCT P4
#define C0 BIT0
#define C1 BIT1
#define C2 BIT2
#define ROW_STRUCT P2
#define R0 BIT4
#define R1 BIT5
#define R2 BIT6
#define R3 BIT7
#define ONE BIT9
#define TWO BITA
#define THREE BITB
#define FOUR BIT6
#define FIVE BIT7
#define SIX BIT8
#define SEVEN BIT3
#define EIGHT BIT4
#define NINE BIT5
#define STAR BIT0
#define ZERO BIT1
#define POUND BIT2

/** \brief Initializes the pins required for keypad operation.
 *
 *  Initializes C0 through C2 as pull-up inputs and R0-R3 as outputs defaulting to 0.
 *  \return void
 *
 */
void key_init();

/** \brief Checks the whole keypad and returns a short containing which key is pressed
 *
 * This function calls checkRow() 4 times from the R0 to R3. The results of each check row
 * is bitshifted over by the next result, making the 12LSB hold all of the relevant data.
 *
 * \return location of all keys pressed encoded in 12 bits of uint16_t
 */
uint16_t checkKP();

/** \brief Checks a single row in the keypad
 *
 *  This function checks a single row of the keypad and returns the position of all of
 *  keys pressed in that row.
 *
 *  \param row macro of the row you want the requested row to be
 *  \return masked byte representing location of key pressed
 */
uint8_t checkRow(uint8_t row);

/** \brief converts the column encoded data into a char
 *
 * This function is intended to take the result from checkKP() and turn it into a
 * character for use in a string or for comparison. The function uses the macros above
 * to determine which character should be returned. If multiple keys are logged or if
 * an error occurs, then the function will return the '?' character.
 *
 * \param bitIndex result of checkKP()
 * \return A usable character for printing or comparison
 */
uint8_t bitConvert(uint16_t bitIndex);//converts 16 bit index into keypad values



#endif /* KEYPAD_H_ */
