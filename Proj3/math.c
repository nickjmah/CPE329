/*
 * math.c
 *
 *  Created on: May 18, 2018
 *      Author: Nick
 */

#include "math.h"

uint32_t sqrtDMM(uint32_t val)
{
    uint32_t i=MSB-1;
    uint32_t guess = 1<<(MSB-1);
    uint32_t check;
    while(i<MSB)
    {
        check = guess* guess;//square
        if(val>check)
        {
            guess |= 1<<i;
            check = guess * guess;
        }
        if(val < check)
        {
            guess &= ~(1<<i);
            guess |= (1<<(i-1));
        }
        else if(val == check)
        {
            return guess;
        }
        i--;
    }
    return guess;
}


