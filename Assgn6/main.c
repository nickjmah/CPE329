#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "timer.h"
#include "lcd.h"
#include "keypad.h"
#include <string.h>
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */


#define FREQ FREQ_12000_KHZ
uint32_t sysFreq = FREQ;///This should be a global variable
uint16_t baud = 1;//SPI frequency in units of KHz
enum mode
{
    count,         /**< timing State */
    wait        /**< wait State */
};
enum mode status=wait;
volatile uint32_t timer_count = 0;
volatile uint32_t result = 0;
volatile uint32_t stuff;

void init(void)
{
    set_DCO(FREQ);
    initSpi(baud);

    // Ensures SLEEPONEXIT takes effect immediately
//    __DSB();
    //     Enable global interrupt
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


