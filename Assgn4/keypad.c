/*
 * keypad.c
 *
 *  Created on: Apr 13, 2018
 *      Author: Jason Zhou
 */
#include "keypad.h"

void key_init(){
    //initializing columns as pull ups
    COL_STRUCT->DIR &= ~(C0|C1|C2);
    COL_STRUCT->REN |= C0|C1|C2;
    COL_STRUCT->OUT &= ~(C0|C1|C2);
    //initializing rows as outputs
    ROW_STRUCT->DIR |= R0|R1|R2|R3;
    ROW_STRUCT->OUT &= ~(R0|R1|R2|R3);
}

uint8_t checkRow(uint8_t row){
    //check all columns of a single row
    ROW_STRUCT->OUT |= row;//enable the selected row
    uint8_t col = COL_STRUCT->IN;//read all of the columns
    col &= (C0|C1|C2);//clear everything but the columns of interest
    ROW_STRUCT->OUT &= ~row;//turn off row
    return col;
}

uint16_t checkKP(){
    //checks all of the rows and bit shifts them into a 16 bit number
    uint8_t i;
    uint16_t result=0;
    for(i=0; i<4; i++){
        result |= checkRow(R0<<i);//checks each row
        result = result << 3;//bit shifts the number over by the amount of columns(3)
    }
    result = result >> 3;//counteracting the last shift
    return result;
}

uint8_t bitConvert(uint16_t bitIndex){
    //case statement to convert the bits into chars
    switch(bitIndex){
    case ONE: return '1';
    case TWO: return '2';
    case THREE: return '3';
    case FOUR: return '4';
    case FIVE: return '5';
    case SIX: return '6';
    case SEVEN: return '7';
    case EIGHT: return '8';
    case NINE: return '9';
    case STAR: return '*';
    case ZERO: return '0';
    case POUND: return '#';
    default: return '?';
    }
}
