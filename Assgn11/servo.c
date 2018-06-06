/*
 * servo.c
 *
 *  Created on: Jun 6, 2018
 *      Author: Jason Zhou
 */
#include "servo.h"

void servoInit(void)
{
    P6 -> SEL0 |= BIT6 | BIT7;          //TA2_3 configuration
    P6 -> SEL1 &= ~(BIT6 | BIT7);
    P6 -> DIR |= BIT6 | BIT7;

    TIMER_A2 -> CCR[0] = 60000; //period
    TIMER_A2 -> CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;   // reset/set
    TIMER_A2 -> CCR[3] = 1500;  //2.5% duty cycle initial setting for 0 degrees
    TIMER_A2 -> CTL = TIMER_A_CTL_SSEL__SMCLK |   //choose ACLK
                      TIMER_A_CTL_MC__UP |   //choose Up mode
                      TIMER_A_CTL_CLR;    //choose CLR
}



