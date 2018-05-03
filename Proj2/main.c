#include "msp.h"
#include <stdio.h>
#include "dco.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "sin.h"
#include "spi.h"
#include "dac.h"
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
#define BAUD 20000

uint32_t masterCount = 0;
uint16_t dutyCycle;
uint8_t timerFlag=0;
extern uint32_t sysFreq=FREQ_48000_KHZ;

void init(void)
{
    set_DCO(sysFreq);
    initSpi(BAUD);
    //Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCR[0] = NUMMMMM;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
                    TIMER_A_CTL_MC__CONTINUOUS; //TODO:check for prescaler
    __enable_irq();
    return;
}
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
    dacOut(sinUpdate(masterCount));
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	void(*wavePtrArr[])(uint16_t, uint16_t) = {square, sawtooth, sine};
    uint8_t functionIndex = SQUARE;
    uint16_t keyPressed=0;
    uint8_t incAmt=1; //represents the scaler to set frequency for the output waveform
    while(1)
    {
        (*wavePtrArr[functionIndex])(MIN_VAL,MAX_VAL);
        if(P4->IFG & (C0|C1|C2))
        {
            keyPressed = checkKP();
            switch(keyPressed){
            case ONE    :   incAmt=1;
                            break;
            case TWO    :   incAmt=2;
                            break;
            case THREE  :   incAmt=3;
                            break;
            case FOUR   :   incAmt=4;
                            break;
            case FIVE   :   incAmt=5;
                            break;
//          case SIX    :   //add if needed;
            case SEVEN  :   functionIndex = SQUARE;
                            break;
            case EIGHT  :   functionIndex = SINE;
                            break;
            case NINE   :   functionIndex = SAWTOOTH;
                            break;
            case STAR   :   if(dutyCycle > MIN_DUTY_CYCLE) //decrement if greater than 10%
                            (dutyCycle -= TEN_PERCENT_DUTY_CYCLE);
                            break;
            case ZERO   :   dutyCycle = HALF_DUTY_CYCLE;
                            break;
            case POUND  :   if(dutyCycle < MAX_DUTY_CYCLE) //increment if less than 90%
                            dutyCycle += TEN_PERCENT_DUTY_CYCLE;
                            break;
            default     :   ;//do nothing otherwise
            }
        }
        masterCount += incAmt;
        while(!timerFlag);//wait to synchronize DAC
        timerFlag = 0;
    }
}

void TA0_0_IRQHandler(void) {
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        timerFlag = 1;
        TIMER_A0->CCR[0] += COUNT_50US_12MHZ;
    }
}
