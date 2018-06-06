#include "msp.h"
#include "dco.h"
#include "delay.h"
#include "freq.h"
#include "timer.h"
#include "keypad.h"
#include "servo.h"

uint32_t sysFreq = FREQ_3000_KHZ; //set system frequency to 48MHz


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
    uint16_t checkValue = 0;
    uint16_t keyPressed = 0;
    uint16_t position = 0;
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
                position = bitConvertInt(keyPressed) * 10;
                while(!(P4->IFG & (C0 | C1 | C2)));
                keyPressed = checkKP();
                P4->IFG &= ~(C0 | C1 | C2 );
                checkValue = bitConvertInt(keyPressed);
                if(checkValue <= 8)
                {
                    position += checkValue;
                    TIMER_A2 -> CCR[3] = MIN_VAL + (position * INCR);
                }
                break;
            case ZERO :
                position = 0;
                while(!(P4->IFG & (C0 | C1 | C2)));
                keyPressed = checkKP();
                P4->IFG &= ~(C0 | C1 | C2 );
                checkValue = bitConvertInt(keyPressed);
                if(checkValue <= 9)
                {
                    position += checkValue;
                    TIMER_A2 -> CCR[3] = MIN_VAL + (position * INCR);
                }
                break;
            case POUND :
                if (TIMER_A2 -> CCR[3] < MAX_VAL) //increment if less than 90%
                    TIMER_A2 -> CCR[3] += INCR;
                break;
            case STAR :
                if (TIMER_A2 -> CCR[3] > MIN_VAL) //increment if less than 90%
                    TIMER_A2 -> CCR[3] -= INCR;
            default:
                break; //do nothing otherwise
            }
        }

        P1->OUT &= ~BIT0;//lower the clock sync
    }
}
