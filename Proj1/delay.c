/** \file delay.c
 * \brief forces the microcontroller to delay for some time
 *
 * Contains functions that can delay the microcontroller for either a certain amount of ms, or us.
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
    cycle = (ms * freq) / CYCLES_MS ; //# of ms * frequency in kHz * 10 / 109 (while loop clock division)
    while(cycle)
    {
        cycle--;
        asm("");
    }
    return;
}

void delay_us(uint32_t us, uint32_t freq)
{
    uint32_t cycle; //define clock cycle
    cycle = (us * freq - OFFSET_US) / (CYCLE_US); //# of us * frequency in kHz - 40000 /10900
    while(cycle)
    {
        cycle--;
        asm("");
    }
    return;
}
