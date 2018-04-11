#include "msp.h"
#include "lcd.h"
#include "dco.h"
#include "stdint.h"
/**
 * main.c
 */
void main(void)
{
    P3->DIR |= (RS|RW|EN);
    P4->DIR = 0xf0;
    P3->OUT &= ~(BIT5|BIT6|BIT7); //clearing enable port?
    uint32_t freq = FREQ_48000_KHZ;
    set_DCO(freq);
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	halfBitInit();
	funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
    funcSet(ENABLE4BIT,ENABLE2LINE,NORMALFONT);
	dispOnSet(DISPLAY_ON, CURSOR_ON, CURSOR_BLINK);

	clearDisplay();
	delay_ms(3, FREQ_48000_KHZ);
	setEntryMode(0x02, 0);
	delay_ms(1, FREQ_48000_KHZ);
	writeData(0b01000001);
	return;
}
