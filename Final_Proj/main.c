#include "msp.h"
#include "dco.h"
#include "delay.h"
#include "freq.h"
#include "hx711.h"
#include "timer.h"

/**
 * main.c
 */

uint32_t sysFreq = FREQ_48000_KHZ; //set system frequency to 48MHz

void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initHX711();
}

void main(void)
{
    uint32_t data = 0;
    init();
    while(1){
    tare(20);
    data = getValue(10);
    }
}
