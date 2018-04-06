#include "msp.h"
#include "delay.h"
#include "freq.h"
#include <stdint.h>
#include "dco.h"
/**
 * main.c
 */
void main(void)
{
    uint32_t sysFreq = FREQ_12_MHz; //setting system frequency to be 12MHz
    //set_DCO(sysFreq);
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    // P1.0 set as GPIO
    P1->SEL0 &= ~BIT0;               // Clear bit 0 of the P1->SEL0 register
    P1->SEL1 &= ~BIT0;               // Clear bit 0 of the P1->SEL1 register

    P1->DIR |= BIT0;                 // P1.0 set as output

    while (1)                        // continuous loop
    {
        P1->OUT ^= BIT0;                // Toggle P1.0 LED
        delay_ms(500, sysFreq);    // Delay
    }
}
