#include "msp.h"
#include "delay.h"
#include <dco.h>
#include "freq.h"

int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;    // Stop WDT

    // P1.0 set as GPIO
    P1->SEL0 &= ~BIT0;               // Clear bit 0 of the P1->SEL0 register
    P1->SEL1 &= ~BIT0;               // Clear bit 0 of the P1->SEL1 register

    P1->DIR |= BIT0;                 // P1.0 set as output

    uint32_t freq = FREQ_24000_KHZ;
    set_DCO(freq);

    P1->OUT |= BIT0;                // Toggle P1.0 LED
    delay_us(2, freq);
    P1->OUT &= ~BIT0;                // Toggle P1.0 LED

}

