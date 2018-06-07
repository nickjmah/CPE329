#include "msp.h"
#include "dco.h"
#include "delay.h"
#include "freq.h"
#include "hx711.h"
#include "timer.h"
#include "scale.h"
#include "lcd.h"
#include "keypad.h"
/**
 * main.c
 */
#define TEN_SECONDS 2
//#define devel 1
//#define out 1
#define test 1
#define numAvg 20
#define CCR_INCR COUNT_10MS_12MHZ//TODO: change this to be a value that corresponds to the correct freq
uint32_t sysFreq = FREQ_12000_KHZ; //set system frequency to 48MHz
volatile uint32_t enterSleep = 0; //TODO: change to enum maybe
volatile uint32_t timerCounter = 0;
/** \brief helper function to enter sleep mode */
void sleep(void);
/** \brief Initializes the all of the unused pins to be inputs to save power */
void boardInit(void);
/** \brief Initalizes the sleep timer */
void initSleepTimer(void);
/** \brief Initializes all peripherals */
void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    boardInit();     //turn off everything to save power
    set_DCO(sysFreq);
    key_init();
    halfBitInit();
    initHX711();
    initScale();
    initSleepTimer();
    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 31);
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    __enable_irq();
}
/** \brief main functions */
void main(void)
{
#ifdef devel
    float data = 0;
    init();
    tare(10);
    calibrate(23.91);
    while(1)
    {
        data = getUnits(10);
        powerDown();
        delay_ms(1000, sysFreq);
    }
#elif out
    init();
    uint32_t weight = 0;
    while(1)
    {
        tare(numAvg);
        weight = getValue(numAvg);

        if(enterSleep)
        {
            sleep();
        }
    }

#elif test
    /** enum to describe the different states of the state machine */
    typedef enum mode
    {
        weigh, units, height, zero, cal
    } mode_t;
    uint16_t* keyRecorded = 0;//pointer to an array of keypresses
    uint32_t tmp = 0;//temp variable used for height measurements
    float tmp_f = 0;//temp variable used for calibration
    mode_t currentMode = weigh; //sets the inital state to weight
    init();
    updateScale(); //starts off with an initial weight measurement
    while (1)
    {
        if(enterSleep)
            sleep();
        //on keypress
        if (checkPress())//look to change modes if a keypress occurs
        {
            enterSleep = 0;
            keyRecorded = getKeyArr();
            switch (currentMode)
            {
            case weigh: //if weighing, check to see if we need to change modes
                switch (*keyRecorded)
                {
                case ONE :
                    currentMode = units;
                    break;
                case TWO :
                    currentMode = height;
                    tmp = 0;
                    break;
                case THREE :
                    currentMode = cal;
                    break;
                case ZERO :
                    currentMode = zero;
                    break;
                default:
                    currentMode = weigh;
                    break;
                }
                break;
            default:
                break;
            }
        }
        //looping mode changes
        switch (currentMode)
        {
        case weigh:
            updateScale();
            break;
            //update units
        case units:
            updateUnits();//set display to units menu
            while (!checkPress())//wait until 1 keypress
                ;
            keyRecorded = getKeyArr();
            switch (*keyRecorded)
            {//1 puts you in kg, 2 puts you in lbs
            case ONE :
                updateSI();
                break;
            case TWO :
                updateImp();
                break;
            default:
                break;//TODO: check to see if you want to talk about invalid keypresses
            }
            currentMode = weigh;
            break;
        case height:
            updateHeightFt(); //prompt for entering feet
            while (getArrSize() < 1)//getting height in feet
                ;
            keyRecorded = getKeyArr();
            tmp = bitConvertInt(*keyRecorded) * 12;//tmp is in inches so mult by 12
            delay_ms(100, sysFreq);
            updateHeightIn(); //prompt for entering inches
            while (getArrSize() < 2)
                ;
            keyRecorded = getKeyArr();
            tmp += bitConvertInt(keyRecorded[0]) * 10 + bitConvertInt(keyRecorded[1]);//add in inches
            changeHeight(tmp);//update height
            currentMode = weigh;
            delay_ms(100, sysFreq);//add in a delay to see the last typed character
            checkPress();//clearing the check press since we already cleared the queue
            break;
        case zero:
            tare(10);//tare will zero the scale
            currentMode = weigh;
            break;
        case cal:
            calScreen();
            while (getArrSize() < 2)//wait for the first 2 keypresses
                ;
            keyRecorded = getKeyArr();
            tmp_f = bitConvertInt(keyRecorded[0]) * 10
                    + bitConvertInt(keyRecorded[1]);//add the first two presses
            writeData('.');//print a '.' in order to show that you are now in grams
            while (getArrSize() < 2)//wait for two presses
                ;
            keyRecorded = getKeyArr();
            tmp_f += ((float) bitConvertInt(keyRecorded[0])) / 10
                    + ((float) bitConvertInt(keyRecorded[1])) / 100;//convert to float and add in grams
            calibrate(tmp_f);//run calibration calcs
            checkPress();//clearing check press
            currentMode = weigh;
            delay_ms(100, sysFreq);
            break;
        default:
            break;
        }
    }

#endif/*devel*/

}
void sleep(void)
{
    // Enter LPM0
    __sleep();
    __no_operation();
}
void boardInit(void)
{
    // GPIO Port Configuration for lowest power configuration
    P1->OUT = 0x00;
    P1->DIR = 0xFF;
    P2->OUT = 0x00;
    P2->DIR = 0xFF;
    P3->OUT = 0x00;
    P3->DIR = 0xFF;
    P4->OUT = 0x00;
    P4->DIR = 0xFF;
    P5->OUT = 0x00;
    P5->DIR = 0xFF;
    P6->OUT = 0x00;
    P6->DIR = 0xFF;
    P7->OUT = 0x00;
    P7->DIR = 0xFF;
    P8->OUT = 0x00;
    P8->DIR = 0xFF;
    P9->OUT = 0x00;
    P9->DIR = 0xFF;
    P10->OUT = 0x00;
    P10->DIR = 0xFF;
    PJ->OUT = 0x00;
    PJ->DIR = 0xFF;
}
void initSleepTimer(void)
{
#ifdef test
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt
    TIMER_A0->CCR[0] = CCR_INCR;//set initial increment
    //set timer to SMCLK, continuous mode, no prescaler
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK |//set timer to ACLK to run even slower
    TIMER_A_CTL_MC__CONTINUOUS | TIMER_A_CTL_ID__8;
//    TIMER_A0->EX0 |= TIMER_A_EX0_TAIDEX_7;
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);//attach interrupt
#endif
}
void TA0_0_IRQHandler(void)
{
    if (TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        TIMER_A0->CCR[0] += CCR_INCR;
        if (timerCounter >= TEN_SECONDS)
        {
            enterSleep = 1; //sleep after 10s of inactivity, otherwise, increment counter
            timerCounter = 0;
        }
        else
        {
            timerCounter += 1;
        }
    }
}

