/*
 * keypad.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Jason Zhou
 */
#include "keypad.h"

void key_init(){
    COL_STRUCT->DIR &= ~(C0|C1|C2);
    COL_STRUCT->REN |= C0|C1|C2;
    COL_STRUCT->OUT |= C0|C1|C2;
    ROW_STRUCT->DIR |= R0|R1|R2|R3;
    ROW_STRUCT->OUT &= ~(R0|R1|R2|R3);
}

void key_loop(){

}

