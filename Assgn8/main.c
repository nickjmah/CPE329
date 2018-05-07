#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"

/**
 * main.c
 */

///Setting global MCLK frequency
uint32_t sysFreq = FREQ_48000_KHZ;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(sysFreq);
	initUART();
	uint8_t receive;
    // Enable global interrupt
    __enable_irq();
    while(1)
    {
        if(readUARTRxFlag())
        {
            receive = readUART();
            sendUART(&receive,1);
        }
    }
}

