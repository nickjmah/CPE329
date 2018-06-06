#include "msp.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	//kill watchdog
    P6 -> SEL0 |= BIT6 | BIT7;          //TA2_3 configuration
    P6 -> SEL1 &= ~(BIT6 | BIT7);
    P6 -> DIR |= BIT6 | BIT7;

    TIMER_A2 -> CCR[0] = 32765 -1; //period
    TIMER_A2 -> CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;   // reset/set
    TIMER_A2 -> CCR[3] = 24574;
    TIMER_A2 -> CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;   // reset/set
    TIMER_A2 -> CCR[4] = 8191;
    TIMER_A2 -> CTL = TIMER_A_CTL_SSEL__ACLK;    //choose ACLK
//                      TIMER_A_CTL_    //choose Up mode
//                      TIMER_A_CTL_    //choose CLR

    __sleep();
    __no_operation();
}
