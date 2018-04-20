#include "msp.h"
#include "keypad.h"
#include "delay.h"
#include "dco.h"
#include "lcd.h"
#include "combo.h"
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
    uint8_t keyArray[ 4 ];
    displayLockedScreen();
    int i = 0;
    while(1)
    {
        keysPressed = checkKP();//check to see if any keys are pressed
        delay_us(50, FREQ);
        if((keysPressed != prev)&&keysPressed)//if the key pressed is different from before and on the rising edge
        {
            if(checkAsterisk(keysPressed)){
                prev=keysPressed;
                i=0;
                memset(keyArray, '/0', 4);
                continue;
            }
            keyArray[i] = bitConvert(keysPressed);
            writeData(keyArray[i]);//write the key position
            i += 1;
            if(i>=4)
            {
                if (checkCode(keyArray))
                {
                    displayUnlockedScreen();
                }
                else
                {
                    displayLockedScreen();
                    i=0;
                    memset(keyArray, '/0', 4);//setting keyArray to be empty
                }
            }


        }
        prev=keysPressed;//set previous key
    }
}
