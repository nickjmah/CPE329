/*
 * keypad.h
 *
 *  Created on: Apr 13, 2018
 *      Author: Jason Zhou
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "msp.h"
#define COL_STRUCT P4
#define C0 BIT0
#define C1 BIT1
#define C2 BIT2
#define ROW_STRUCT P2
#define R0 BIT4
#define R1 BIT5
#define R2 BIT6
#define R3 BIT7

void key_init();
uint8_t checkRow(uint8_t );



#endif /* KEYPAD_H_ */
