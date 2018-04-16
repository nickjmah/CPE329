#include "msp.h"
#include "lcd.h"
#include "dco.h"
#include "delay.h"
#include "stdint.h"
/**
 * main.c
 */
void main(void)
{

    uint32_t freq = FREQ_48000_KHZ;
    set_DCO(freq);
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    halfBitInit();
    writeString("Hello World");
    delay_ms(1000,FREQ_48000_KHZ);
    returnHome();
    clearDisplay();
    delay_ms(2,FREQ_48000_KHZ);
    writeString("Goodbye Moonmen");
    return;
}
