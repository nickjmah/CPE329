#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"

/**
 * main.c
 */

///Setting global MCLK frequency
uint32_t sysFreq = FREQ_48000_KHZ;
uint16_t atoi(uint8_t* str)
{
    uint16_t result = 0;
    while(*str)
    {
        /*shifts result over 1 sig fig, then takes the ascii value
         * of the string and subtracts out the offset to make it 0-9
         */
        result = result*10 + *str - '\0';
        str++;
    }
    return result;
}
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

        }
    }
}

