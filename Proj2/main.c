#include "msp.h"
#include <stdio.h>
#include "dco.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "sin.h"
#include "spi.h"
/**
 * main.c
 */

#define SQUARE 0
#define SAWTOOTH 1
#define SINE 2
#define MAXMASTERCOUNT 1599
#define HALF_DUTY_CYCLE 1
#define TEN_PERCENT_DUTY_CYCLE 1
#define MAX_DUTY_CYCLE 1
#define MIN_DUTY_CYCLE 1
void init(void)
{
    ;
}
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
    uint16_t keysPressed=0;
    uint8_t incAmt=1; //represents the scaler to set frequency for the output waveform
    while(1)
    {
        if(P4->IFG & (C0|C1|C2))
        {
            keysPressed = checkKP();
            switch(keyPressed){
            case ONE    :   incAmt=1;
            case TWO    :   incAmt=2;
            case THREE  :   incAmt=3;
            case FOUR   :   incAmt=4;
            case FIVE   :   incAmt=5;
//          case SIX    :   //add if needed;
            case SEVEN  :   /*square wave*/;
            case EIGHT  :   /*sine wave*/;
            case NINE   :   /*saw wave*/;
            case STAR   :   dutyCycle > MIN_DUTY_CYCLE ? //decrement if greater than 10%
                            (dutyCycle -= TEN_PERCENT_DUTY_CYCLE) : ;
            case ZERO   :   dutyCycle < MAX_DUTY_CYCLE ? //increment if less than 90%
                            dutyCycle = HALF_DUTY_CYCLE : ;
            case POUND  :   dutyCycle += TEN_PERCENT_DUTY_CYCLE;
            default     :   ;//do nothing otherwise
            }
        }
    }
}
