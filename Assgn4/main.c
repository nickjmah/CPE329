#include "msp.h"


/**
 * main.c
 */

#include "keypad.h"

void main(void)
{
	key_init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint8_t col0,col1,col2;
    while(1){
        col0 = checkRow(R0);
        col1 = checkRow(R1);
        col2 = checkRow(R2);
    }
}
