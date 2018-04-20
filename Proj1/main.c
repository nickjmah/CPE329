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
    int womboCombo = "1234";
    int comboSize = strlen(WOMBO_COMBO);

	init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint16_t keysPressed, prev=0;
    uint8_t keyArray[ comboSize ];
    displayLockedScreen();
    int i = 0;
    while(1)
    {
        keysPressed = checkKP();//check to see if any keys are pressed
        delay_us(100, FREQ);
        if((keysPressed != prev)&&keysPressed)//if the key pressed is different from before and on the rising edge
        {
            if(checkAsterisk(keysPressed)){
                prev=keysPressed;
                i=0;
                memset(keyArray, '/0', comboSize);
                continue;
            }
            keyArray[i] = bitConvert(keysPressed);
            writeData(keyArray[i]);//write the key position
            i += 1;
            if(i>=comboSize)
            {
                delay_ms(100,FREQ);
                if (checkCode(keyArray))
                {
                    displayUnlockedScreen();
                }
                else
                {
                    displayLockedScreen();

                }
                i=0;
                memset(keyArray, '/0', comboSize);//setting keyArray to be empty
            }
        }
        prev=keysPressed;//set previous key
    }
}
