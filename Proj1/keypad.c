/** @file keypad.c
 * @brief Read inputs from a 12-key keypad
 *
 * This file will reads inputs for from the keypad using pins defined in keypad.h
 * and can either return an encoded result of all keys pressed at the time of measurement
 * or can return a single character of a single key pressed.
 *
 * @author Nick Mah
 * @author Jason Zhou
 *
 *
 */
#include "keypad.h"

void key_init(){
    //initializing columns as pull ups
    COL_STRUCT->DIR &= ~(COL_MASK);     //set columns to inputs
    COL_STRUCT->REN |= COL_MASK;        //enable pull-x circuitry to columns
    COL_STRUCT->OUT &= ~(COL_MASK);     //set columns as pull-ups
    //initializing rows as outputs
    ROW_STRUCT->DIR |= ROW_MASK;     //set rows as outputs
    ROW_STRUCT->OUT &= ~(ROW_MASK);  //set all rows as 0
}

uint8_t checkRow(uint8_t row){
    //check all columns of a single row
    ROW_STRUCT->OUT |= row;//enable the selected row
    uint8_t col = COL_STRUCT->IN;//read all of the columns
    col &= (COL_MASK);//clear everything but the columns of interest
    ROW_STRUCT->OUT &= ~row;//turn off row
    return col; //returns column results in row
}

uint16_t checkKP(){
    //checks all of the rows and bit shifts them into a 16 bit number
    int i; //init iterator for the for loop
    uint16_t result=0; //init final result
    for(i=0; i<4; i++){//define for loop used to check all of the rows
        result |= checkRow(R0<<i);//checks row at iterator
        result = result << 3;//bit shifts the number over by the amount of columns(3)
    }
    result = result >> 3;//counteracting the last shift
    return result; //returns final result
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
    default: return '?'; //if the bit encoded value is invalid or more than one key, return ?
    }
}
