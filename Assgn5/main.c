#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include <string.h>
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */


#define FREQ FREQ_12000_KHZ

enum mode
{
    count,         /**< timing State */
    wait        /**< wait State */
};
enum mode status=wait;
volatile uint32_t timer_count = 0;
volatile uint32_t result = 0;
volatile uint32_t stuff;
/** \brief turns a number into a string
 *
 *
 */
char* itoa(int val)
{
    static char buf[32] = {0};
    int i = 30;
    for(; val && i; i--, val/=10)
        buf[i] = "0123456789"[val % 10];
    return &buf[i+1];
}
void init(void)
{
    set_DCO(FREQ);
    halfBitInit();
    key_init();
    ROW_STRUCT->OUT |=R3;//enabling bottom row for checking
    // Configure keypad for interrupts
    COL_STRUCT->IE |= COL_MASK; //enables interrupts
    COL_STRUCT->IES &= ~COL_MASK; //set interrupt to trigger on rising edge
    COL_STRUCT->IFG &= ~COL_MASK; //clear lingering flags
    //Configure Timer for interrupts
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_100_1500KHZ;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS;

//    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
//    __DSB();
    //     Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 31);//enabling port 1 interrupt
    return;
}
#include "msp.h"

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
            WDT_A_CTL_HOLD;
    init();
    while (1)
    {
                __sleep();

                __no_operation();
    }
}

// Timer A0 interrupt service routine

void PORT4_IRQHandler(void)
{
    if (P4->IFG &C0)
    {
        P4->IFG &= ~C0;
        timer_count = 0; //clear the counter
        result = 0; //clear result
        status = count;
        TIMER_A0->CCR[0] = TIMER_A0->R+COUNT_1MS_12MHZ;//set the counter to go in 1ms

    }
    if(P4->IFG & C2)
    {
        P4->IFG&= ~C2;
        if(status==count)
        {
            status=wait;
            result = timer_count; //set the result to be the timer
            clearDisplay();
            writeString(itoa(result));
        }
    }
    return;
}
void TA0_0_IRQHandler(void) {
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        timer_count += 1;
        TIMER_A0->CCR[0] += COUNT_1MS_12MHZ;              // Add Offset to TACCR0
    }
}
