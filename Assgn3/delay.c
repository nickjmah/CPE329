/*
 * delay.c
 *
 *  Created on: Apr 4, 2018
 *      Author: Jason Zhou
 *
 *  *speed optimization must be level 5
 */

#include "delay.h"

void delay_ms(uint32_t ms, uint32_t freq)
{
    uint32_t cycle; //define clock cycle
    cycle = (ms * freq) * 10 / (109); //# of ms * frequency in kHz * 10 / 109 (while loop clock division)
    while(cycle)
    {
        cycle--;
    }
    return;
}

void delay_us(uint32_t us, uint32_t freq)
{
    uint32_t cycle; //define clock cycle
    cycle = (us * freq - 40000) / (10900); //# of us * frequency in kHz - 40000 /10900
    while(cycle)
    {
        cycle--;
    }
    return;
}

