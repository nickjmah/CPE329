#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "timer.h"
#include "keypad.h"
#include <string.h>
#include "spi.h"
#include "dac.h"
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */


#define FREQ FREQ_12000_KHZ

uint32_t sysFreq = FREQ;///This should be a global variable
uint16_t baud = 6000;//SPI frequency in units of KHz
//enum mode
//{
//    low,         /**< timing State */
//    high        /**< wait State */
//};
//enum mode status=wait;
volatile uint8_t output=0;
volatile uint32_t stuff;

void init(void)
{
    set_DCO(FREQ);
    initSpi(baud);

    // Ensures SLEEPONEXIT takes effect immediately
//    __DSB();
    //     Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_20MS_12MHZ;    //TODO: this is too big of a number:(add in a prescaler
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS |
            TIMER_A_CTL_ID__4;

//    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
//    __DSB();
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    __enable_irq();//TODO: check to see if NVIC can be set before this

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
void TA0_0_IRQHandler(void) {
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        if(!output)
            dacOut(DAC_MAX_VAL);
        else
            dacOut(DAC_MIN_VAL);
        output ^= 1; //invert state
        TIMER_A0->CCR[0] += COUNT_20MS_12MHZ;   //TODO:Change this to the correct offset // Add Offset to TACCR0
    }
    return;
}
#if 0
    void init(void)
    {
        set_DCO(FREQ);
        initSpi(baud);

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        //     Enable global interrupt
        TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
        TIMER_A0->CCR[0] = COUNT_20MS_12MHZ;
        TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                TIMER_A_CTL_MC__CONTINUOUS;

    //    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
        __enable_irq();//TODO: check to see if NVIC can be set before this

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
    void TA0_0_IRQHandler(void) {
        if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
        {
            TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
            if(!output)
                dacOut(DAC_MAX_VAL);

            else
                dacOut(DAC_MAX_VAL);
            output ^= 1; //invert state
            TIMER_A0->CCR[0] += COUNT_20MS_12MHZ;   //TODO:Change this to the correct offset // Add Offset to TACCR0
        }
#endif
