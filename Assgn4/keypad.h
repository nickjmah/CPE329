/*
 * keypad.h
 *
 *  Created on: Apr 13, 2018
 *      Author: Jason Zhou
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
void key_init();
uint16_t checkKP();//returns a 16 bit integer stuffed with booleans representing each key that's pressed
uint8_t checkRow(uint8_t row);//returns 3 bits of bools of all col numbers in a row
uint8_t bitConvert(uint16_t bitIndex);//converts 16 bit index into keypad values



#endif /* KEYPAD_H_ */
