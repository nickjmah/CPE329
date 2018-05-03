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
#define CCR_INCR 300
#define HALF_DUTY_CYCLE 800
#define TEN_PERCENT_DUTY_CYCLE 160
#define MAX_DUTY_CYCLE 1440
#define MIN_DUTY_CYCLE 160
#define BAUD 20000

uint32_t masterCount = 0;
uint16_t dutyCycle = HALF_DUTY_CYCLE;
uint8_t timerFlag=0;
uint8_t port4Flag = 0;
extern uint32_t sysFreq=FREQ_48000_KHZ;

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
//    NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 31);//enabling port 1 interrupt
    COL_STRUCT->IFG &= ~COL_MASK;
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
    init();
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
    while(1)
    {
        (*wavePtrArr[functionIndex])(DAC_MIN_VAL,DAC_MAX_VAL);
        if(P4->IFG & (C0|C1|C2))
//        if(port4Flag)
        {
//            port4Flag = 0;
            P4->IFG &= ~(C0|C1|C2);
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
        }
        masterCount += incAmt;
        if(masterCount >= 1600)
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

//void PORT4_IRQHandler(void) {
//    P4->IFG &= ~(C0|C1|C2);
//    ROW_STRUCT->OUT &= ~ROW_MASK; //setting all rows low
//    P1->OUT |= BIT0;
//    port4Flag = 1;
//    P1->OUT &= ~BIT0;
//}
