/*
 * delay.c
 *
 *  Created on: Apr 4, 2018
 *      Author: Nick
 */
#include "delay.h"
#include "dco.h"

int delay_ms(int ms, uint16_t freq_set)
{
    int i, ticks;
    int freq = 3; //frequency in Mhz
    if(ms<0){//returns -1 if there is an invalid clock frequency or a negative time
        return -1;
    }
    ticks = (ms * freq) * MHZ_IN_HZ / MS_IN_S;//conversion to ticks
    for (i = 0; i < ticks; i ++);

    return 1;
}

int delay_us(int us, uint16_t freq_set)
{
    int i, ticks;
    int freq = 3;
    if(us<0){
        return -1;
    }
    ticks = (us * freq) * MHZ_IN_HZ / US_IN_S;
    for (i = 0; i < ticks; i++);
    return 1;
}

