/** \file delay.c
 * \brief forces the microcontroller to delay for some time
 *
 * Contains functions that can delay the microcontroller for either a certain
 * amount of ms, or us.
 *
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#include "delay.h"

void delay_ms(uint32_t ms, uint32_t freq)
{
    uint32_t cycle; //define clock cycle
    cycle = (ms * freq) / CYCLES_MS ;   //multiply the time by frequency to get
                                        //ideal amount of clock cycles
                                        //then divide by the number of actual
                                        //cycles in the while loop

    while(cycle)    //run in the loop for theoretical amount of cycles
    {
        cycle--;
        asm("");    //do an assembly no op so the loop doesn't get deleted
    }
    return;
}

void delay_us(uint32_t us, uint32_t freq)
{
    uint32_t cycle; //define clock cycle
    cycle = (us * freq - OFFSET_US) / (CYCLE_US);
    //same as delay_ms except subtract by an offset to compensate for
    //calculation clocks
    while(cycle)
    {
        cycle--;
        asm("");    //assembly no op to stop deletion by compiler
    }
    return;
}
