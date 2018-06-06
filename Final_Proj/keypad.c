/** @file keypad.c
 * @brief Read inputs from a 12-key keypad
 *
 * This file will reads inputs for from the keypad using pins defined in keypad.h
 * and can either return an encoded result of all keys pressed at the time of
 * measurement or can return a single character of a single key pressed.
 *
 * @author Nick Mah
 * @author Jason Zhou
 *
 *
 */
#include "keypad.h"

void key_init(){
    //initializing columns as pull ups
    P4->DIR &= ~((C0|C1|C2));     //set columns to inputs
    P4->REN |= (C0|C1|C2);        //enable pull-x circuitry to columns
    P4->OUT &= ~((C0|C1|C2));     //set columns as pull-downs
    //initializing rows as outputs
    P2->DIR |= (R0|R1|R2|R3);     //set rows as outputs
    P2->OUT |= ((R0|R1|R2|R3));  //set all rows as 0
    //enabling interrupts on all columns
    P4->IE |= (C0|C1|C2);
    P4->IES &= ~(C0|C1|C2);
    P4->IFG &= ~(C0|C1|C2);
    ///Not enabling NVIC because just checking the flags in a if statement


}

uint8_t checkRow(uint8_t row){
    //check all columns of a single row
    delay_ms(5, sysFreq);
    P2->OUT |= row;//enable the selected row
    uint8_t col = P4->IN;//read all of the columns
    col &= ((C0|C1|C2));//clear everything but the columns of interest
    P2->OUT &= ~(R0|R1|R2|R3);//turn off row
    return col; //returns column results in row
}

uint16_t checkKP(){
    //checks all of the rows and bit shifts them into a 16 bit number
    int i; //init iterator for the for loop
    P2->OUT &= ~(R0 | R1 | R2 | R3 ); //setting all rows low so a measurement can start
    uint16_t result=0; //init final result
    for(i=0; i<4; i++){//define for loop used to check all of the rows
        result |= checkRow(R0<<i);//checks row at iterator
        result = result << 3;
                        //bit shifts the number over by the amount of columns(3)
    }
    result = result >> 3;//counteracting the last shift
    P2->OUT |= (R0|R1|R2|R3);//setting all rows high for key interrupt
    return result; //returns final result
}

char bitConvertChar(uint16_t bitIndex){
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
    //if the bit encoded value is invalid or more than one key, return ?
    }
}
uint32_t bitConvertInt(uint16_t bitIndex){
    //case statement to convert the bits into chars
    switch(bitIndex){
    case ZERO: return 0;
    case ONE: return 1;
    case TWO: return 2;
    case THREE: return 3;
    case FOUR: return 4;
    case FIVE: return 5;
    case SIX: return 6;
    case SEVEN: return 7;
    case EIGHT: return 8;
    case NINE: return 9;
    default: return 1000;
    /*if the bit encoded value is invalid or more than one key, return 1000
     should generate a math error
     */
    }
}
