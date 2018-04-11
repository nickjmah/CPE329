#include "msp.h"
#include "lcd.h"
#include "dco.h"
#include "stdint.h"
/**
 * main.c
 */
void main(void)
{
    uint32_t freq = FREQ_48000_KHZ;
    set_DCO(freq);
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);


}
