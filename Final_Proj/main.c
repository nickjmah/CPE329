#include "msp.h"
#include "dco.h"
#include "delay.h"
#include "freq.h"
#include "hx711.h"
#include "timer.h"

/**
 * main.c
 */
//#define devel 1
#define out 1
#define numAvg 20
uint32_t sysFreq = FREQ_48000_KHZ; //set system frequency to 48MHz
uint32_t enterSleep = 0;//TODO: change to enum maybe
void sleep(void);
void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initHX711();
    initScale();
}

void main(void)
{
#ifdef devel
    uint32_t data = 0;
    init();
    while(1){
    tare(20);
    data = getValue(10);
    }
#elif out
    init();
    uint32_t weight = 0;
    while(1)
    {
        tare(numAvg);
        weight = getValue(numAvg);
        if(enterSleep)
        {
            sleep();
        }
    }


#endif/*devel*/

}
void sleep(void)
{
    // Enter LPM0
    __sleep();
    __no_operation();
}
