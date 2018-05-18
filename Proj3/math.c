/*
 * math.c
 *
 *  Created on: May 18, 2018
 *      Author: Nick
 */

#include "math.h"

uint32_t sqrtDMM(uint32_t val)
{
    int i=MSB-1;
    uint32_t guess = 1<<(MSB-1);
    uint32_t check;
    while(i<MSB)
    {
        check = guess* guess;//square
        if(val>check)
        {
            guess |= 1<<i;
        }
        else if(val < check)
        {
            guess &= ~(1<<i);
            guess |= (1<<(i-1));
        }
        else
        {
            return guess;
        }
        i--;
    }
    return guess;
}


