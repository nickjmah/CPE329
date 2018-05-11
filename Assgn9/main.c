#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"
#include "adc.h"

/**
 * main.c
 */
uint32_t sysFreq = FREQ_24000_KHZ;
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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initUART();
    initADC();
}

void main(void)
{
    uint8_t ADC_Val;
    init();

    // Enable global interrupt
    __enable_irq();

    while (1)
    {

        // Start sampling/conversion
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        while(!readADCFlag()){
            asm(""); //prevent while loop from being compiled out at higher optimizations
        }
        if(readADCFlag())
        {
            ADC_Val = readADC();
            sendUART(&ADC_Val, sizeof(ADC_Val));
        }
    }
}


