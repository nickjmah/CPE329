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
    uint8_t i;
    uint16_t result=0;
    for(i=0; i<4; i++){
        result |= checkRow(R0<<i);
        result = result << 3;
    }
    result = result >> 3;
    return result;
}

uint8_t bitConvert(uint16_t bitIndex){
    switch(bitIndex){
    case 1: return '#';
    case 2: return '0';
    case 4: return '*';
    case 8: return '9';
    case 16: return '8';
    case 32: return '7';
    case 64: return '6';
    case 128: return '5';
    case 256: return '4';
    case 512: return '3';
    case 1024: return '2';
    case 2048: return '1';
    default: return '\0';
    }
}
