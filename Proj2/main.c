#include "msp.h"
#include <stdio.h>
#include "dco.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "sin.h"
#include "spi.h"
#include "dac.h"
/** \file main.c
 * @brief main file for function generator
 *
 * This project creates a function generator that can create a square,
 * triangle, sawtooth, or sine wave at 100-500Hz in increments of 100Hz.
 * Duty cycle is adjustable by increments of 10% between 10 and 90%.
 * All settings are controlled through the keypad.
 *
 * @author Nick Mah
 * @author Jason Zhou
 *
 */

#define SQUARE 0 //function pointer address for the square wave
#define SAWTOOTH 1
#define SINE 2
#define TRIANGLE 3
#define KHZ_PER_100HZ 10 //converts 1Khz in units of 100Hz for unit conv.
///The maximum count masterCount should reach
#define MAX_MASTER_COUNT (uint16_t)(FREQ_48000_KHZ*KHZ_PER_100HZ/CCR_INCR)
#define HALF_MAX_COUNT MAX_MASTER_COUNT / 2
///The increment by which TimerA should trigger and write to DAC
#define CCR_INCR 450
///Point in masterCount where 50% dutycycle occurs
#define HALF_DUTY_CYCLE (MAX_MASTER_COUNT + 1)/ 2
#define TEN_PERCENT_DUTY_CYCLE (MAX_MASTER_COUNT) / 10 + 1
#define MAX_DUTY_CYCLE (MAX_MASTER_COUNT) - TEN_PERCENT_DUTY_CYCLE
#define MIN_DUTY_CYCLE TEN_PERCENT_DUTY_CYCLE
#define BAUD 16000

///Setting global MCLK frequency
uint32_t sysFreq = FREQ_48000_KHZ;
//Function prototypes
void square(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
            uint16_t maxVal, uint8_t incr);
void sine(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
            uint16_t maxVal, uint8_t incr);
void sawtooth(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
            uint16_t maxVal, uint8_t incr);
void triangle(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
            uint16_t maxVal, uint8_t incr);
//Initializing function pointer array
void (*wavePtrArr[])(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t) =
{   square, sawtooth, sine, triangle };

//Global Variable Declarations
uint8_t functionIndex = SQUARE; //set the start waveform as a square wave
uint16_t keyPressed = 0;//initialize keypressed
uint8_t incAmt = 1; //represents the scaler to set frequency for the output waveform
uint32_t masterCount = 0; //initialize counter
uint16_t dutyCycle = HALF_DUTY_CYCLE; //set duty cycle to half


/** \brief Initializes all functions and enables timers
 *
 * \return void
 */
void init(void)
{

    set_DCO(sysFreq);
    initSpi(BAUD);
    key_init();
    //Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt
    TIMER_A0->CCR[0] = CCR_INCR;            //set initial increment
    //set timer to SMCLK, continuous mode, no prescaler
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
            TIMER_A_CTL_MC__CONTINUOUS;
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);//attach interrupt
    P4->IFG &= ~(C0 | C1 | C2 ); //clear P4 interrupt flag
    __enable_irq();
    P1->DIR |= BIT0; //initialize clock sync
    P1->OUT &= ~BIT0;
    return;
}
/** \brief Sends a single bit of a square wave
 *
 *  \param masterCount Timer used for setting the frequency
 *  \param dutyCycle The duty cycle of the square wave, unused in other functions
 *  \param minVal The minimum value the DAC can output
 *  \param maxVal The maximum value the dac can output
 *  \param incr The variable that sets the output waveform frequency
 *  \return void
 */
void square(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
            uint16_t maxVal, uint8_t incr)
{
    if (masterCount == 0)
    {
        dacOut(maxVal);
    }
    else if (masterCount == (dutyCycle - dutyCycle % incr))
    {
        dacOut(minVal);
    }
}
/** \brief Sends a single bit of a square wave
 *
 *  This function takes in the value of master count and maps it by changing the slope of it
 *  to map to the max dacOut value. This is because masterCouter looks like a sawtooth when
 *  plotted.
 *
 *  \param masterCount Timer used for setting the frequency
 *  \param dutyCycle The duty cycle of the square wave, unused in other functions
 *  \param minVal The minimum value the DAC can output
 *  \param maxVal The maximum value the dac can output
 *  \param incr The variable that sets the output waveform frequency
 *  \return void
 */
void sawtooth(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
              uint16_t maxVal, uint8_t incr)
{
    dacOut(maxVal * masterCount / MAX_MASTER_COUNT);
}
/** \brief Sends a single bit of a sine wave
 *
 *  This function uses sinUpdate to map masterCount to a sinewave, then sends it to the DAC
 *  \param masterCount Timer used for setting the frequency
 *  \param dutyCycle The duty cycle of the square wave, unused in other functions
 *  \param minVal The minimum value the DAC can output
 *  \param maxVal The maximum value the dac can output
 *  \param incr The variable that sets the output waveform frequency
 *  \return void
 */
void sine(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
          uint16_t maxVal, uint8_t incr)
{
    dacOut(sinUpdate(masterCount));
}
/** \brief Sends a single bit of a square wave
 *
 *  This function is identical to sawtooth() except that it inverts the value of masterCount
 *  once the we reach half of masterCount
 *  \param masterCount Timer used for setting the frequency
 *  \param dutyCycle The duty cycle of the square wave, unused in other functions
 *  \param minVal The minimum value the DAC can output
 *  \param maxVal The maximum value the dac can output
 *  \param incr The variable that sets the output waveform frequency
 *  \return void
 */
void triangle(uint16_t masterCount, uint16_t dutyCycle, uint16_t minVal,
              uint16_t maxVal, uint8_t incr)
{
    if (masterCount <= HALF_MAX_COUNT)
        dacOut(2 * maxVal * masterCount / MAX_MASTER_COUNT);
    else
        dacOut(2 * maxVal * (MAX_MASTER_COUNT - masterCount) / MAX_MASTER_COUNT);
}
/** \brief Main function
 *  The main function is only responsible for initialization and checking
 *  the keypad. There is an infinite loop that constantly checks for
 *  a rising edge in the keypad. All rows are turned on in order for this
 *  to be possible. If there is an interrupt, the keypad is checked and
 *  the appropriate action is determined through the case statement.
 *  buttons 1-5 control the frequency from 100-500Hz by changing the
 *  amount that masterCount increments by. 6-9 control the index of the
 *  wavePtrArr, which controls the output waveform. *-# decrements, sets to
 *  50% or increments the duty cycle as long as the dutyCycle states
 *  above 10% and below 90%.
 *  \return void
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    init();

    while (1)
    {
        if (P4->IFG & (C0 | C1 | C2 )) //check if a key is pressed
        {
            P2->OUT &= ~(R0 | R1 | R2 | R3 ); //setting all rows low so a measurement can start
            keyPressed = checkKP(); //read the  keypad for the exact key pressed
            P4->IFG &= ~(C0 | C1 | C2 );
            /*clear any interrupts, including any caused by
             *key pressed*/
            switch (keyPressed) //mapping each key pressed to a specific action
            {
            case ONE :
                incAmt = 1;
                break;
            case TWO :
                incAmt = 2; //increasing incAmt causes mastercount to reset by incAmt * 100 Hz
                break;
            case THREE :
                incAmt = 3;
                break;
            case FOUR :
                incAmt = 4;
                break;
            case FIVE :
                incAmt = 5;
                break;
            case SIX :/*cause the function pointer array to point to the triangle function*/
                functionIndex = TRIANGLE;
                break;
            case SEVEN :
                functionIndex = SQUARE;
                break;
            case EIGHT :
                functionIndex = SINE;
                break;
            case NINE :
                functionIndex = SAWTOOTH;
                break;
            case STAR :
                if (dutyCycle > MIN_DUTY_CYCLE) //decrement if greater than 10%
                    (dutyCycle -= TEN_PERCENT_DUTY_CYCLE);
                break;
            case ZERO :
                dutyCycle = HALF_DUTY_CYCLE; //set the dutyCycle to 50%
                break;
            case POUND :
                if (dutyCycle < MAX_DUTY_CYCLE) //increment if less than 90%
                    dutyCycle += TEN_PERCENT_DUTY_CYCLE;
                break;
            default:
                break; //do nothing otherwise
            }
        }

        P1->OUT &= ~BIT0;//lower the clock sync
    }
}

/** \brief Timer Interrupt handler
 *
 *  This function will be regularly called every CCR_INCR/48MHz seconds.
 *  When called, the IRQ will set itself to be called again after the same amount of time.
 *  Then, the function will send a SPI command to the DAC that is determined by the
 *  index fed to wavePtrArr. Next, the masterCount, which is what is used to keep
 *  track of the period is incremented or zero'd depending on whether or not it is
 *  at its max value. Finally, the clock sync pulse is raised whenever masterCount is reset.
 *
 *  \return void
 */
void TA0_0_IRQHandler(void)
{
    if (TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //clear the interrupt flag
        TIMER_A0->CCR[0] += CCR_INCR; //increment the CCR register to time again
        /*outputs a value to the DAC that is mapped to a value based off of the master count
         * that corresponds to a waveform indicated by the function index
         */
        (*wavePtrArr[functionIndex])(masterCount, dutyCycle, DAC_MIN_VAL,
                DAC_MAX_VAL, incAmt);
        masterCount += incAmt; //increment the masterCounter
        if (masterCount >= MAX_MASTER_COUNT)
        {
            masterCount = 0; //reset the masterCounter if it exceeds the mapped value
            P1->OUT |= BIT0; //set the clock sync high
        }
    }
}
