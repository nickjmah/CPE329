/** \file lcd.h
 * \brief Contains function prototypes for lcd functionality
 *
 * This file contains function prototypes and macros for everything needed to
 * define the lcd. All commands sent are in 4-bit mode, with the exception of
 * halfBitInit()
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#ifndef LCD_H_
#define LCD_H_
#include <stdint.h>
#include "msp.h"
#include "../Assgn2/delay.h"
#include "../Assgn2/dco.h"
#include "string.h"


//R/S pin map
#define RS BIT5

///R/W pin map
#define RW BIT6

///Enable pin map
#define EN BIT7

///Command bit to clear display
#define CLEAR_DISP 0x01

///Command bit to return home
#define RET_HOME 0x02

///Command bit to set the entry mode
#define ENTRY_MODE_SET 0x04

///Command bit to to change display settings
#define DISP_CTRL 0x08

///Command bit to change cursor and display settings
#define CURS_DISP_SHFT 0x10

///Command bit to enable function set
#define FUNCSET 0x20

///Command bit to set CGRAM address
#define CGRAM 0x40

///Command bit to set DDRAM address
#define DDRAM 0x80

///Setting bit to cause the cursor to shift
#define CURS_SHFT 0x00

///Setting bit to cause the display to shift
#define DISP_SHFT 0x08

///Setting bit to set the shift direction to right
#define RIGHT_SHFT 0x04

///Setting bit to set the shift direction to left
#define LEFT_SHFT 0x00

///Address of second row in DDRAM
#define ROW_SHFT 0x40

///Setting bit to enable 4 bit mode
#define ENABLE4BIT 0x00

///Setting bit to enable 8 bit mode
#define ENABLE8BIT 0x10

///Setting bit to enable 2 line mode
#define ENABLE2LINE 0x08

///Setting bit to enable 1 line mode
#define ENABLE1LINE 0x00

///Setting bit to enable normal font
#define NORMALFONT 0x04

///Setting bit to turn on the display
#define DISPLAY_ON 0x04

///Setting bit to turn on the cursor
#define CURSOR_ON 0x02

///Setting bit to turn off the cursor
#define CURSOR_OFF 0x00

///Setting bit to make the cursor blink
#define CURSOR_BLINK 0x01

///Setting bit to turn off cursor blink
#define CURSOR_NO_BLINK 0x00

///Setting bit to set the cursor to shift right
#define CURSOR_INCREMENT_RIGHT 0x02
/** \brief Initializes the display in 4-bit mode
 *
 *  This function sends all of the commands required to initialize the lcd in
 *  4-bit mode. The general flow is as follow:\n
 *  1. Do Function Set to 4 bit mode in 8-bit mode\n
 *  2. Do Function Set in 4-bit mode with all configuration bits desired\n
 *  3. Do Function Set in 4-bit mode again with all congif bits desired\n
 *  4. Do Display On/Off in 4-bit mode with all settings desired\n
 *  5. Do Display Clear in 4-bit mode
 *  6. Do entry mode set in 4-bit mode with all settings desired
 *
 *  \return void
 */
void halfBitInit();

/** \brief Writes a string to the lcd
 *
 * This function calls writeData for each character in the string.
 * The function takes a char array pointer as an argument
 *
 * \param string a char array pointer to a string
 * \return void
 */
void writeString(char* string);

/** \brief Sends a command to the lcd in 4-bit mode
 *
 *  This function sends a 4-bit mode command to the LCD. The function takes in a
 *  char array pointer and iterates through the string until the char value is a
 *  null character
 *
 *  \param cmd The desired command to be sent
 *  \return void
 */
void writeCommand(uint8_t cmd); //sends a command to the lcd

/** \brief Sends a command to clear the display
 * \return void
 */
void clearDisplay();

/** \brief Sends a command to return the cursor to home
 *
 * \return void
 */
void returnHome();

/** \brief Sets direction of the cursor and display shift
 *
 * \param direction Sets the direction the cursor will shift
 * \param dispShift Sets the direction how the display will shift
 * \return void
 */
void setEntryMode(uint32_t direction, uint32_t dispShift);

/** \brief sets whether or not the various parts of the display are on
 * \param disp Macro to set whether or not the display is on
 * \param cursor Macro to whether or not the cursor is visible
 * \param cursorBlink Macro to set whether or not the cursor will blink
 * \return void
 */
void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink);

/** \brief Shifts the cursor or the the display
 *
 * \param cursorShift Macro to set whether to shift the cursor or display
 * \param direction Macro to set which direction it will be shifted
 * \return void
 */
void shift(uint32_t cursorShift, uint32_t direction);

/** \brief Sets general parameters for the display
 *
 * \param dataLen Macro to set 4-bit vs 8-bit mode
 * \param numLines Macro to set the number of lines of 2 or 1 line
 * \param font set the font as 5x11 vs 5x8
 * \return void
 */
void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font);

/** \brief Sets the CGRAM address
 *
 * \param address Address of the CGRAM to be set
 * \return void
 */
void setCGRAM(uint32_t address);

/** \brief Set the DDRAM address
 *
 * \param address Address of the DDRAM to be set
 */
void setDDRAM(uint32_t address);

/** \brief Check to see if the busy flag is set or not
 *
 * \return A byte that is true if the busy flag is high
 * \bug This doesn't work yet. Do not use. This is because of readData() not
 * working
 */
uint8_t checkBusy();

/** \brief Writes a character to the display
 *
 * This set the R/S bit high, then sends the data through the writeCommand() with
 * the data. Then, the R/S bit is set back low
 *
 * \param data The desired payload to be send to the display
 * \return void
 */
void writeData(uint32_t data);

/** \brief Reads the data at a specific RAM address
 *
 * \return The data that's to being read from the lcd
 * \bug This doesn't work, please ignore this code
 */
uint8_t readData();

/** \brief Shifts the cursor down to the second row, first column
 *
 * This function sets the DDRAM address to be a the second row and first column
 * \return void
 */
void rowShiftDown();
#endif /* LCD_H_ */
//
