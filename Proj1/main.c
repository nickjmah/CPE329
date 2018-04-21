/** \file main.c
 * \brief Runs a combination lock for MSP432
 *
 * This file contains the main function used to operate the combination lock.
 * initializes all of the pre-requisite features and then polls for key changes.
 * If there is a key change, then the loop checks to see if it detected a
 * rising or falling edge. In the event of a rising edge, then the loop will
 * add number to an array of key presses.Once the number of key presses hits
 * COMBO_SIZE, main will check to see whether or not the code matches. If the
 * code matches, then an unlocked screen will appear and the loop restarts.
 * If the number of key presses hits COMBO_SIZE again, then womboCombo will be
 * updated. At any point, '*' the lock will lock again and the keyArray
 * will reset.
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
/** Enum representing the state of the lock
 * This enum has two states that changes the operation of the lock
 */
enum mode
{
    locked,         /**< Locked State */
    unlocked        /**< Unlocked State */
};

/** \brief initializes all peripherals required for the MSP432
 *
 * This function initializes the DCO to FREQ, initializes the LCD,
 * and initializes the keypad.
 *
 * \return void
 */
void init(void)
{
    set_DCO(FREQ);  //setting DCO frequency
    halfBitInit();  //Initialize LCD
    key_init();     //Initialize keypad

}

/** \brief Main function
 *
 *  \return void
 */
void main(void)
{
    uint8_t womboCombo[4] = "1234";     //initializing the combo
    int i=0;
    int j = 0;                        //iterator for copying

    enum mode status=locked;            //creating enum for lock state

    init();                             //init everything
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    uint16_t keysPressed, prev=0;
    uint8_t keyArray[ COMBO_SIZE ];     //defining array to hold key presses
    displayLockedScreen();

    while(1)
    {
        keysPressed = checkKP();//check to see if any keys are pressed
        delay_us(100, FREQ);
        if((keysPressed != prev)&&keysPressed)
            /* if the key pressed is different from before
             * and on the rising edge
             */
        {
            if(checkAsterisk(keysPressed)){
                /* if asterisk is pressed, clear the key press history and
                 * start over from the locked screen
                 */
                prev=keysPressed;
                i=0;
                status=locked;                      //set the status to locked
                memset(keyArray, 'a', COMBO_SIZE);
                //sets every value of the key array to an impossible value
                continue;//skip the rest of the code and restart while loop
            }
            keyArray[i] = bitConvert(keysPressed);//add the key press
            writeData(keyArray[i]); //write the key to the screen
            i += 1;                 //change the iterator to the next key value
            if(i>=COMBO_SIZE)
            //check if the keyArray is the correct size
            {

                delay_ms(100,FREQ);
                if(status==unlocked)
                {
                    /*if the lock is unlocked set the code to be whatever was
                     * typed
                     */
                    for(j=0; j<COMBO_SIZE; j++)
                    {
                        womboCombo[j] = keyArray[j];
                        //copying each element of the array over
                    }
                    displayLockedScreen();
                    status=locked;//change the lock back to being locked
                }
                else if (checkCode(keyArray, womboCombo))
                {
                    /*if the lock isn't locked and the code matches,
                     *unlock the lock
                     */
                    displayUnlockedScreen();
                    status=unlocked;
                }
                else
                {
                    /*the key must not match now.
                     * display the lock screen and change status to locked
                     * just in case of an issue
                     */
                    displayLockedScreen();
                    status=locked;

                }
                i=0;
                memset(keyArray, 'a', COMBO_SIZE);
                //setting keyArray to be 'a'
            }
        }
        prev=keysPressed;//set previous key
    }
}
