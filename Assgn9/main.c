#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"
#include "adc.h"

/**
 * main.c
 */
#define ADC_MAX_VAL 16383
#define SIG_FIGS 3
uint32_t sysFreq = FREQ_48000_KHZ;
uint8_t* itoaForADC(int val)
{
    int adc = val*330/ADC_MAX_VAL;
    static uint8_t buf[32] = {0};
    int i = 30;
    int j=0;
    for(j=0; j<SIG_FIGS+1; i--,j++)
    {
        if(j==SIG_FIGS-1)
        {
            buf[i] = '.';
        }
        else
        {
            buf[i] = "0123456789"[(adc) % 10];
            adc /= 10;
        }
    }
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
    uint32_t ADC_Val;
    init();

    // Enable global interrupt
    __enable_irq();

    while (1)
    {

        // Start sampling/conversion
        startConv();
        while(!readADCFlag()){
            asm(""); //prevent while loop from being compiled out at higher optimizations
        }
        ADC_Val = readADC();
        sendUART(itoaForADC(ADC_Val), SIG_FIGS+1);
        sendUART("\r\n",2);
    }
}


