/*
 * timer.c
 *
 *  Created on: May 14, 2018
 *      Author: Jason Zhou
 */

#include "timer.h"
#include "msp.h"

void initTimer(void)
{
    //Configure Timer for interrupts
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_20US_48MHZ;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                    TIMER_A_CTL_MC__CONTINUOUS;
}



