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
//	dispOnSet(DISPLAY_ON, CURSOR_ON, CURSOR_BLINK);
//	clearDisplay();
//	delay_ms(3, FREQ_48000_KHZ);
//	setEntryMode(0x02, 0);
	writeData(0b00010100);
	return;
}
