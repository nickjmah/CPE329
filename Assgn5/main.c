#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "timer.h"
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#define test

#define FREQ FREQ_1500_KHZ

volatile uint32_t timer_count = 0;

void init(void)
{
//    P4->DIR |= BIT3;
//    P4->SEL1&= ~BIT3;
//    P4->SEL0|= BIT3;

    set_DCO(FREQ);
    return;
}
#include "msp.h"

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;

    init();

    // Configure GPIO
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;
    P2->DIR |= BIT2;
    P2->OUT |= BIT2;

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_100_1500KHZ;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enabled
    TIMER_A0->CCR[1] = COUNT_100_1500KHZ;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
//#ifndef test
//     Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
//#endif
    while (1)
    {
//        __sleep();

        __no_operation();                   // For debugger
    }
}

// Timer A0 interrupt service routine

void TA0_0_IRQHandler(void) {
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        P1->OUT ^= BIT0;
        TIMER_A0->CCR[0] += COUNT_1_1500KHZ;              // Add Offset to TACCR0
    }
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
        P2->OUT ^= BIT2;
        TIMER_A0->CCR[1] += COUNT_2_1500KHZ;              // Add Offset to TACCR0
    }
}
