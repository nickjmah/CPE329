#include "msp.h"
#include "keypad.h"
#include "delay.h"
#include "dco.h"
#include "lcd.h"
#define FREQ FREQ_48000_KHZ

/**
 * main.c
 */


void init(void)
{
    set_DCO(FREQ);//setting DCO frequency
    halfBitInit();//Initialize LCD
    key_init();//Initialize keypad

}
void main(void)
{

	init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint16_t keysPressed, prev=0;
    while(1)
    {
        keysPressed = checkKP();//check to see if any keys are pressed
        if((keysPressed != prev)&&keysPressed)//if the key pressed is different from before and on the rising edge
        {
            writeData(bitConvert(keysPressed));//write the key position
            returnHome();//return cursor so it next press erases it

        }
        prev=keysPressed;//set previous key
    }
}
