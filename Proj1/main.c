/** \file main.c
 * \brief Runs a combination lock for MSP432
 *
 * This file contains the main function used to operate the combination lock. The file can
 * initializes all of the pre-requisite features and then polls for key changes. If there
 * is a key change, then the loop checks to see if it detected a rising or falling edge
 * In the event of a rising edge, then the loop will add number to an array of key presses.
 * Once the number of key presses hits COMBO_SIZE, main will check to see whether or not
 * the code matches. If the code matches, then an unlocked screen will appear and the loop
 * restarts. If the number of key presses hits COMBO_SIZE again, then comboWombo will be updated
 * If at any point, '*' the lock will lock again and the keyArray will reset.
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
#include "msp.h"
#include "keypad.h"
#include "delay.h"
#include "dco.h"
#include "lcd.h"
#include "combo.h"

/// Sets the operating frequency of the system
#define FREQ FREQ_48000_KHZ

/** \brief initializes all peripherals required for the MSP432
 *
 * This function initializes the DCO to FREQ, initilalizes the LCD, and initializes the keypad
 *
 * \return void
 */
void init(void)
{
    set_DCO(FREQ);//setting DCO frequency
    halfBitInit();//Initialize LCD
    key_init();//Initialize keypad

}

/** \brief Main function
 *
 *  \return void
 */
void main(void)
{
    uint8_t womboCombo[4] = "1234";
    int j;
    enum mode {locked, unlocked};
    enum mode status=locked;
	init();
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint16_t keysPressed, prev=0;
    uint8_t keyArray[ COMBO_SIZE ];
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
                status=locked;
                memset(keyArray, '/0', COMBO_SIZE);
                continue;
            }
            keyArray[i] = bitConvert(keysPressed);
            writeData(keyArray[i]);//write the key position
            i += 1;
            if(i>=COMBO_SIZE)
            {

                delay_ms(100,FREQ);
                if(status==unlocked)
                {
                    for(j=0; j<COMBO_SIZE; j++)
                    {
                        womboCombo[j] = keyArray[j];
                    }
                    displayLockedScreen();
                    status=locked;
                }
                else if (checkCode(keyArray, womboCombo))
                {
                    displayUnlockedScreen();
                    status=unlocked;
                }
                else
                {
                    displayLockedScreen();
                    status=locked;

                }
                i=0;
                memset(keyArray, '/0', COMBO_SIZE);//setting keyArray to be empty
            }
        }
        prev=keysPressed;//set previous key
    }
}
