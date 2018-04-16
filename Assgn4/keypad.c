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
    COL_STRUCT->OUT &= ~(C0|C1|C2);
    ROW_STRUCT->DIR |= R0|R1|R2|R3;
    ROW_STRUCT->OUT &= ~(R0|R1|R2|R3);
}

uint8_t checkRow(uint8_t row){
    ROW_STRUCT->OUT |= row;
    uint8_t col = COL_STRUCT->IN;
    col &= (C0|C1|C2);
    ROW_STRUCT->OUT &= ~row;
    return col;
}

uint16_t checkKP(){

}

