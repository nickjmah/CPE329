#include "msp.h"
#include <stdio.h>


/**
 * main.c
 */

#define SQUARE 0
#define SAWTOOTH 1
#define SINE 2
#define MAXMASTERCOUNT 1599

uint32_t masterCount = 0;
uint16_t dutyCycle;

void square(uint16_t minVal, uint16_t maxVal)
{
    if(masterCount == 0){
        dacOut(maxVal);
    }
    else if(masterCount == dutyCycle){
        dacOut(minVal);
    }
}

void sawtooth(uint16_t minVal, uint16_t maxVal)
{
    dacOut(maxVal * masterCount / MAXMASTERCOUNT);
}

void sine(uint16_t minVal, uint16_t maxVal)
{

}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	void(*wavePtrArr[])(uint16_t, uint16_t) = {square, sawtooth, sine};

}
