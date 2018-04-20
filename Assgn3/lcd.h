/** \file lcd.h
 * \brief Contains function prototypes for delay functionality
 *
 * This file contains two functions. One can delay in microseconds and the other in milliseconds
 *
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
#define RS BIT5
#define RW BIT6
#define EN BIT7
#define CLEAR_DISP 0x01
#define RET_HOME 0x02
#define ENTRY_MODE_SET 0x04
#define DISP_CTRL 0x08
#define CURS_DISP_SHFT 0x10
#define CURS_SHFT 0x00
#define DISP_SHFT 0x08
#define RIGHT_SHFT 0x04
#define LEFT_SHFT 0x00
#define FUNCSET 0x20
#define CGRAM 0x40
#define DDRAM 0x80
#define ROW_SHFT 0x40
#define ENABLE4BIT 0x00
#define ENABLE8BIT 0x10
#define ENABLE2LINE 0x08
#define ENABLE1LINE 0x00
#define NORMALFONT 0x04
#define DISPLAY_ON 0x04
#define CURSOR_ON 0x02
#define CURSOR_OFF 0x00
#define CURSOR_BLINK 0x01
#define CURSOR_NO_BLINK 0x00
void halfBitInit();
void writeString(char* string);
void writeCommand(uint8_t cmd, uint8_t init); //sends a command to the lcd
void clearDisplay();
void returnHome();
void setEntryMode(uint32_t direction, uint32_t dispShift);
void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink);
void shift(uint32_t cursorShift, uint32_t direction);
void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font);
void setCGRAM(uint32_t address);
void setDDRAM(uint32_t address);
uint8_t checkBusy();
void writeData(uint32_t data);
uint8_t readData();
void rowShiftDown();
#endif /* LCD_H_ */
//
