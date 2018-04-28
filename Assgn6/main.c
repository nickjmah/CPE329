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
enum mode
{
    add,         /**< timing State */
    subtract        /**< wait State */
};
enum mode status=add;
volatile uint8_t output=0;
volatile uint32_t stuff;
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
            dacOut(voltageOut(0.1));
        else
            dacOut(DAC_MIN_VAL);
        output ^= 1; //invert state
        TIMER_A0->CCR[0] += COUNT_20MS_12MHZ;   //TODO:Change this to the correct offset // Add Offset to TACCR0
        }
    return;
}
#endif
#if 0 ///This is the code for the triangle wave that sucks
    uint8_t writeDac = 0; //says when to write to dac
    uint16_t dacVal;
    void init(void)
    {
        set_DCO(FREQ);
        initSpi(baud);

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        //     Enable global interrupt
        TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
        TIMER_A0->CCR[0] = COUNT_10MS_12MHZ;
        TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                TIMER_A_CTL_MC__CONTINOUS;

    //    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
        __enable_irq();

        return;
    }
    #include "msp.h"

    int main(void) {
        WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                WDT_A_CTL_HOLD;
        init();
        uint16_t tmp;
        while (1)
        {
            if(writeDac)
            {
                if(status == add)
                {
                    if(dacVal >= DAC_2V - (COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ){
                        dacVal = (DAC_2V-(COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ) + dacVal - DAC_2V;
                        status = subtract;
                    }
                    else
                        dacVal += (COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ;
                }
                else
                {
                    if(dacVal <= (COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ){
                        dacVal = (COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ - dacVal;
                        status = add;
                    }
                    else
                        dacVal -= (COUNT_1MS_12MHZ * DAC_2V) / COUNT_10MS_12MHZ;
                }
                dacOut(dacVal);
                writeDac=0;
            }

            //add something to sleep
        }
    }
    void TA0_0_IRQHandler(void) {
        if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
            TIMER_A0->CCTL[0] &= TIMER_A_CCTLN_CCIFG;
            writeDac=1;
            TIMER_A0->CCR[0] += COUNT_1MS_12MHZ;
        }
    }
#endif

#if 0//this is the better version that might not work
    uint8_t writeDac = 0; //says when to write to dac
    uint16_t dacVal;
    void init(void)
    {
        set_DCO(FREQ);
        initSpi(baud);

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        //     Enable global interrupt
        TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
        TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; //TACCR1 interrupt enable
        TIMER_A0->CCR[0] = COUNT_10MS_12MHZ;
        TIMER_A0->CCR[1] = COUNT_1MS_12MHZ;
        TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                TIMER_A_CTL_MC__UPDOWN;

    //    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR

        // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
        NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
        __enable_irq();//TODO:

        return;
    }
    #include "msp.h"

    int main(void) {
        WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                WDT_A_CTL_HOLD;
        init();
        uint16_t tmp;
        while (1)
        {
            if(writeDac)
            {
                dacOut(dacVal);
                writeDac=0;
            }

            //add something to sleep
        }
    }
    void TA0_0_IRQHandler(void) {
        if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
        {
            TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
            TIMER_A0->CCR[0] = COUNT_10MS_12MHZ;
        }
        if(TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)
        {
            dacVal = ((TIMER_A0->R) * DAC_2V) / COUNT_10MS_12MHZ; // this may or may not overflow
            TIMER_A0->CCTL[0] &= TIMER_A_CCTLN_CCIFG; //reading TIMER_A0-> may cause an interrupt to occur instantly. Need to test this
            TIMER_A0->CCTL[1] &= TIMER_A_CCTLN_CCIFG;

            writeDac=1;

            if(TIMER_A0->CCR[1] >=COUNT_10MS_12MHZ-COUNT_1MS_12MHZ)
                TIMER_A0->CCR[1] = COUNT_10MS_12MHZ - TIMER_A0->CCR[1] + COUNT_1MS_12MHZ;
            else
                TIMER_A0->CCR[1] += COUNT_1MS_12MHZ;
        }
    }
#endif

#if 0
void init(void)
{
    set_DCO(FREQ);
    initSpi(baud);

    // Ensures SLEEPONEXIT takes effect immediately
//    __DSB();
    //     Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[0] = 65535;    //TODO: this is too big of a number:(add in a prescaler
    TIMER_A0->CCR[1] = 0;
    TIMER_A0->CCR[2] = 0;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__UPDOWN |
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
    if((TIMER_A0->CCTL[2] & TIMER_A_CCTLN_CCIFG))
    {
        TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIFG;
        if((TIMER_A0->CCTL[1] & TIMER_A_CCTLN_OUT))
            dacOut(voltageOut(0.1));
        else
            dacOut(DAC_MIN_VAL);
        TIMER_A0->CCR[0] += COUNT_20MS_12MHZ;   //TODO:Change this to the correct offset // Add Offset to TACCR0
    }
    return;
}
#endif
