#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"
#include "adc.h"
#include "dmm.h"
#include "vt100.h"
#include "math.h"
#include "pwm.h"
#include "lcd.h"
/**
 * main.c
 */
uint32_t sysFreq = FREQ_48000_KHZ; //set system frequency to 48MHz

void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    halfBitInit();
    initUART();
    initADC();
    initGUI();
    pwmInit();
    initFreqMeas();
}

void main(void)
{
//    uint16_t receive = 0;   //holds the number a key press on the UART Terminal
//    uint32_t offset = 0;    //The DC offset of an AC wave
//    uint32_t vpp = 0;       //The peak-peak voltage
//    uint32_t rms = 0;       //The RMS voltage
//    uint32_t prev;          //The previous result
//    uint32_t ACVals[3] = { 0 };    //An array that holds all of the initial meas
    /*ACVal[0] is the sum of squares of our measurements
     * ACVal[1] is the minimum value
     * ACVal[2] is the maximum value
     */
//    uint32_t* ACVal_ptr = &(ACVals[0]); //creating a pointer to the first element of ACVals
    init();
    __enable_irq();

    while (1)
    {
        if(readFreqFlag())
        {
            updateFreqMeas(calcFreq());
            delay_ms(150, sysFreq);
        }
/*
        if (readUARTRxFlag()) //check for a keyPress
        {
            prev = receive;
            receive = readResult(); //read any keypresses
            clearResult();     //clear the value of result
            if (prev != receive) //Stops the display from suffering from button mashing
            {
                switch (receive)
                {
                case (1):           //If 1 is pressed, enter DC Mode
                    displayDC();
                    break;
                default:            //Else, enter AC Mode
                    displayAC();
                }
            }
        }
        switch (receive)
        {
        case (1):   //Perform DC Measurements is 1 is pressed
            updateVDC(averageDC());     //Take average DC Value after a ms
            break;
        default:    //Perform AC Measurement otherwise
            if (readFreqFlag())    //check if frequency has an update
            {
                updateFreq(calcFreq());
                //update frequency on the screen if there's a new reading
            }
            ACMeas(ACVal_ptr); //take a 1 second burst mesurement and record values
            offset = OffsetCalc(ACVals[1], ACVals[2]); //calculate VAC
            updateVAC(offset);                        //print VAC
            vpp = PTPCalc(ACVals[1], ACVals[2]);      //Calculated Peak to Peak
            updatePkPk(vpp);
            rms = sqrtDMM((ACVals[0]));               //root sum of squares
            updateRMS(rms);
            updateWave(waveDetect(rms, vpp, offset)); //check waveform and update
        }
*/
    }
}

