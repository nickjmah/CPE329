/*
 * lcd.h
 *
 *  Created on: Apr 8, 2018
 *      Author: Nick
 */
#include <stdint.h>
#ifndef LCD_H_
#define LCD_H_

#include "msp.h"
#include "delay.h"

#define RS BIT5
#define RW BIT6
#define EN BIT7

#define CLEAR_DISP 0x01
#define RET_HOME 0x02
#define ENTRY_MODE_SET 0x04
#define DISP_CTRL 0x08
#define CURS_DISP_SHFT 0x10

void writeCommand();
uint32_t readCommand();
void clearDisplay();
void returnHome();
void setEntryMode(uint32_t direction, uint32_t dispShift);
void dispOnSet(uint32_t disp, uint32_t cursor, uint32_t cursorBlink);
void shift(uint32_t cursorShift, uint32_t direction);
void funcSet(uint32_t dataLen, uint32_t numLines, uint32_t font);
void setCGRAM(uint32_t address);
void setDDRAM(uint32_t address);
uint32_t checkBusy();
void writeData(uint32_t data);
uint32_t readData();
#endif /* LCD_H_ */
//
