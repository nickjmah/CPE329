#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "timer.h"
#include "keypad.h"
#include <string.h>
#include "spi.h"
#include "dac.h"
/** \file main.c
 * \brief DAC demo
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */


#define FREQ FREQ_12000_KHZ
#define TRIANGLE_WAVE //enable this for the triangle wave
//#define SQUARE_WAVE //enable this for the square wave
uint32_t sysFreq = FREQ;///This should be a global variable that represents frequency
                          //that can be used anywhere
uint16_t baud = 6000;//SPI frequency in units of KHz
enum mode
{
    add,         /**< Represents op counting state */
    subtract        /**< Represents down counting state */
};
enum mode status=add;
volatile uint8_t output=0;

#ifdef SQUARE_WAVE
void init(void)
{
    set_DCO(FREQ);
    initSpi(baud);

    //     Enable global interrupt
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_20MS_12MHZ;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
            TIMER_A_CTL_MC__CONTINUOUS |
            TIMER_A_CTL_ID__4;

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    __enable_irq();

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
            dacOut(DAC_2V);
        else
            dacOut(DAC_MIN_VAL);
        output ^= 1; //invert state
        TIMER_A0->CCR[0] += COUNT_10MS_12MHZ;// Add Offset to TACCR0
        }
    return;
}
#endif


#ifdef TRIANGLE_WAVE
    uint8_t writeDac = 0; //says when to write to dac
    uint16_t dacVal; //value to write to the dac
    void init(void)
    {
        set_DCO(FREQ);
        initSpi(baud); //initalize spi

        //     Enable global interrupt
        TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
        TIMER_A0->CCR[0] = COUNT_50US_12MHZ;
        TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                TIMER_A_CTL_MC__CONTINUOUS;

        NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
        __enable_irq();

        return;
    }
    #include "msp.h"

    int main(void) {
        WDT_A->CTL = WDT_A_CTL_PW |             // Stop WDT
                WDT_A_CTL_HOLD;
        init();
        while (1)
        {
            if(writeDac) //only write to dac after some time
            {
                if(status == add)
                {
                    //checks to see if the dac increment will go above the allowed value
                    if(dacVal >= DAC_2V - (COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ)
                    {
                        //if it goes above the allowed value, start subtracting
                        dacVal = (DAC_2V-(COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ) +
                                dacVal - DAC_2V;
                        status = subtract;
                    }
                    else
                    {
                        //otherwise, increment at the slope to get 2Vpp 20ms period
                        dacVal += (COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ;
                    }
                }
                else
                {
                    //checks to see if decrementing dac will go negative
                    if(dacVal <= (COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ)
                    {
                        //if it goes below, start adding from zero
                        dacVal = (COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ - dacVal;
                        status = add;
                    }
                    else
                        //otherwise, keep subtracting
                        dacVal -= (COUNT_50US_12MHZ * DAC_2V) / COUNT_10MS_12MHZ;
                }
                //keep writing to dac
                dacOut(dacVal);
                writeDac=0;
            }

        }
    }
    void TA0_0_IRQHandler(void) {
        if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
        {
            TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
            writeDac=1;
            TIMER_A0->CCR[0] += COUNT_50US_12MHZ;
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
        TIMER_A0->CCR[1] = COUNT_50US_12MHZ;
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
            dacVal = (TIMER_A0->R * DAC_2V) / COUNT_10MS_12MHZ
            TIMER_A0->CCTL[0] &= TIMER_A_CCTLN_CCIFG; //reading TIMER_A0-> may cause an interrupt to occur instantly. Need to test this
            TIMER_A0->CCTL[1] &= TIMER_A_CCTLN_CCIFG;

            writeDac=1;

            if(TIMER_A0->CCR[1] >=COUNT_10MS_12MHZ-COUNT_50US_12MHZ)
                TIMER_A0->CCR[1] = COUNT_10MS_12MHZ - TIMER_A0->CCR[1] + COUNT_50US_12MHZ;
            else
                TIMER_A0->CCR[1] += COUNT_50US_12MHZ;
        }
    }
#endif
