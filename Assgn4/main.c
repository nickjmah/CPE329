#include "msp.h"


/**
 * main.c
 */

#include "keypad.h"

void main(void)
{
	key_init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
