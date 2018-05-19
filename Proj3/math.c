/*
 * math.c
 *
 *  Created on: May 18, 2018
 *      Author: Nick
 */

#include "math.h"

uint32_t sqrtDMM(uint32_t val)
{
    uint32_t i = MSB - 1;
    uint32_t guess = 1 << (MSB - 1); //guess is the MSB
    uint32_t check;
    while (i < MSB) //loop until i is less than 0 and underflows
    {
        check = guess * guess; //square guess
        if (val > check)       //if val is greater than check add another bit
        {
            guess |= 1 << i;
            check = guess * guess;       //recalculate check
        }
        if (val < check) //if val is less than check bit shift that made bit right
        {
            guess &= ~(1 << i);
            guess |= (1 << (i - 1));
        }
        else if (val == check)          //if val matches, return result
        {
            return guess;
        }
        i--; //manipulate the next bit
    }
    return guess;
}

