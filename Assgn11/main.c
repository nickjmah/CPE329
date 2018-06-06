#include "msp.h"
#include "dco.h"
#include "delay.h"
#include "freq.h"
#include "timer.h"
#include "keypad.h"
#include "servo.h"

uint32_t sysFreq = FREQ_3000_KHZ; //set system frequency to 3MHz


void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    key_init();
    servoInit();
}

/**
 * main.c
 */
void main(void)
{
    uint16_t checkValue = 0; //second value of position
    uint16_t keyPressed = 0;//unmapped position of keypad press
    uint16_t position = 0; //2 digit result of servo desired position from 0 to 18
	init();
	while (1)
    {
        if (P4->IFG & (C0 | C1 | C2 )) //check if a key is pressed
        {
            keyPressed = checkKP(); //read the  keypad for the exact key pressed
            P4->IFG &= ~(C0 | C1 | C2 );
            /*clear any interrupts, including any caused by
             *key pressed*/
            switch (keyPressed) //mapping each key pressed to a specific action
            {
            case ONE :
                position = bitConvertInt(keyPressed) * 10; //adding initial digit to position
                while(!(P4->IFG & (C0 | C1 | C2))); //blocking loop for second press
                keyPressed = checkKP(); //read the  keypad for second digit
                P4->IFG &= ~(C0 | C1 | C2 );
                /*clear any interrupts, including any caused by
                 *key pressed*/
                checkValue = bitConvertInt(keyPressed); //convert keypress into integer
                if(checkValue <= 8)
                {
                    position += checkValue; //add ones digit to position
                    TIMER_A2 -> CCR[3] = MIN_VAL + (position * INCR);
                    /*adjusting CCR value to the input number
                     *multiplied by the increment value
                     */
                }
                break;
            case ZERO :
                position = 0; //adding initial digit to position
                while(!(P4->IFG & (C0 | C1 | C2))); //blocking loop for second press
                keyPressed = checkKP(); //read the  keypad for second digit
                P4->IFG &= ~(C0 | C1 | C2 );
                /*clear any interrupts, including any caused by
                 *key pressed*/
                checkValue = bitConvertInt(keyPressed);
                if(checkValue <= 9)
                {
                    position += checkValue; //add ones digit to position
                    TIMER_A2 -> CCR[3] = MIN_VAL + (position * INCR);
                    /*adjusting CCR value to the input number
                     *multiplied by the increment value
                     */
                }
                break;
            case POUND :
                if (TIMER_A2 -> CCR[3] < MAX_VAL) //increment if less than 180 degrees
                    TIMER_A2 -> CCR[3] += INCR;
                break;
            case STAR :
                if (TIMER_A2 -> CCR[3] > MIN_VAL) //decrement if more than 0 degrees
                    TIMER_A2 -> CCR[3] -= INCR;
            default:
                break; //do nothing otherwise
            }
        }
    }
}
