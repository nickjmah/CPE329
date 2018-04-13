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
#define ONE BIT0
#define TWO BIT1
#define THREE BIT2
#define FOUR BIT3
#define FIVE BIT4
#define SIX BIT5
#define SEVEN BIT6
#define EIGHT BIT7
#define NINE BIT8
#define STAR BIT9
#define ZERO BIT10
#define POUND BIT11
void key_init();
uint16_t checkKP();//returns a 16 bit integer stuffed with booleans representing each key that's pressed
uint8_t checkRow(uint8_t col);//returns 3 bits of bools of all col numbers in a row



#endif /* KEYPAD_H_ */
