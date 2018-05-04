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
#define KHZ_PER_100HZ 10
#define MAXMASTERCOUNT (uint16_t)(FREQ_48000_KHZ*KHZ_PER_100HZ/CCR_INCR)
#define CCR_INCR 450
#define HALF_DUTY_CYCLE (MAXMASTERCOUNT + 1)/ 2
#define TEN_PERCENT_DUTY_CYCLE (MAXMASTERCOUNT + 1) / 10
#define MAX_DUTY_CYCLE (MAXMASTERCOUNT + 1) - TEN_PERCENT_DUTY_CYCLE
#define MIN_DUTY_CYCLE TEN_PERCENT_DUTY_CYCLE
#define BAUD 16000

uint32_t sysFreq=FREQ_48000_KHZ;
volatile uint8_t timerFlag=0;

void init(void)
{
    set_DCO(sysFreq);
    initSpi(BAUD);
    key_init();
    //Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCR[0] = CCR_INCR;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
                    TIMER_A_CTL_MC__CONTINUOUS; //TODO:check for prescaler
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    P4->IFG &= ~(C0|C1|C2);
    __enable_irq();
    return;
}
void square(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal, uint16_t maxVal, uint8_t incr)
{
    if(masterCount == 0){
        dacOut(maxVal);
    }
    else if(masterCount == (dutyCycle - dutyCycle%incr)){
        dacOut(minVal);
    }
}

void sawtooth(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal, uint16_t maxVal, uint8_t incr)
{
    dacOut(maxVal * masterCount / MAXMASTERCOUNT);
}

void sine(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal, uint16_t maxVal, uint8_t incr)
{
    dacOut(sinUpdate(masterCount));
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	void(*wavePtrArr[])(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t) = {square, sawtooth, sine};
    uint8_t functionIndex = SAWTOOTH;
    uint16_t keyPressed=0;
    uint8_t incAmt=1; //represents the scaler to set frequency for the output waveform
    uint32_t masterCount = 500;
    uint16_t dutyCycle = HALF_DUTY_CYCLE;
    init();
    while(1)
    {
        (*wavePtrArr[functionIndex])(masterCount, dutyCycle, DAC_MIN_VAL,DAC_MAX_VAL, incAmt);
        if(P4->IFG & (C0|C1|C2))
//        if(port4Flag)
        {
//            port4Flag = 0;
            P2->OUT &= ~(R0|R1|R2|R3); //setting all rows low
            keyPressed = checkKP();

            P4->IFG &= ~(C0|C1|C2);
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
                case SIX    :   break;
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
                default     :   break;//do nothing otherwise
            }
//            P1->OUT &= ~BIT0;
        }
        masterCount += incAmt;
        if(masterCount >= MAXMASTERCOUNT)
            masterCount = 0;
        while(!timerFlag);//wait to synchronize DAC
        timerFlag = 0;
    }
}

void TA0_0_IRQHandler(void) {
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        timerFlag = 1;
        TIMER_A0->CCR[0] += CCR_INCR;
    }
}
