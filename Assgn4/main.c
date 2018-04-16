#include "msp.h"


/**
 * main.c
 */

#include "keypad.h"
#include "delay.h"
#include "dco.h"
#include "lcd.h"
#define FREQ FREQ_48000_KHZ
void init(void)
{
    set_DCO(FREQ);
    halfBitInit();
    key_init();

}
void main(void)
{

	init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint16_t keysPressed, prev=0;
    while(1)
    {
        keysPressed = checkKP();
        if((keysPressed != prev)&&keysPressed)
        {
            writeData(bitConvert(keysPressed));
            returnHome();

        }
        prev=keysPressed;
    }
}
