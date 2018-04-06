/*
 * delay.c
 *
 *  Created on: Apr 4, 2018
 *      Author: Nick
 */
#include "delay.h"
#include "dco.h"

int delay_ms(int ms, uint32_t freq_set)
{
    int i, ticks;
    int freq = 3; //frequency in khz
    if(ms<0){//returns -1 if there is an invalid clock frequency or a negative time
        return -1;
    }
    ticks = (ms * freq) * KHZ_IN_HZ / MS_IN_S / CYC_PER_LOOP;//conversion to ticks
    for (i = 0; i < ticks; i ++);

    return void;
}

int delay_us(int us, uint32_t freq_set)
{
    int i, ticks;
    int freq = 3;
    if(us<0){//TODO: add in some sort of thing to say that the loop is not fast enough
        return -1;
    }
    ticks = (us * freq) * KHZ_IN_HZ / US_IN_S / CYC_PER_LOOP;
    for (i = 0; i < ticks; i++);
    return void;
}

