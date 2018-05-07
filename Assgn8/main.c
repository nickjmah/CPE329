#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"
#include "dac.h"
/**
 * main.c
 */

///Setting global MCLK frequency
uint32_t sysFreq = FREQ_48000_KHZ;

/** \brief init everything
 *
 *  Sets DCO to 48MHz, initializes UART and SPI
 */
void init(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initUART();
    initSPI(16000);//set SPI to 16Mbaud
}


void main(void)
{

	uint16_t receive;
	init();
    // Enable global interrupt
    __enable_irq();
    while(1)
    {
        if(readUARTRxFlag())
        {
            receive = readResult();
            if(receive <= DAC_MAX_VAL)//check to make sure the sent val is within bounds
            {
                dacOut(receive);
            }
            clearResult();
        }
    }
}

//uint16_t atoi(uint8_t* str)
//{
//    uint16_t result = 0;
//    while(*str)
//    {
//        /*shifts result over 1 sig fig, then takes the ascii value
//         * of the string and subtracts out the offset to make it 0-9
//         */
//        result = result*10 + *str - '\0';
//        str++;
//    }
//    return result;
//}
