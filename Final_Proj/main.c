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
#define TEN_SECONDS 10000
//#define devel 1
//#define out 1
#define test 1
#define numAvg 20
#define CCR_INCR 10000000000000000//TODO: change this to be a value that corresponds to the correct freq
uint32_t sysFreq = FREQ_12000_KHZ; //set system frequency to 48MHz
volatile uint32_t enterSleep = 0; //TODO: change to enum maybe
volatile uint32_t timerCounter = 0;
void sleep(void);
void boardInit(void);
void initSleepTimer(void);
void changeMode(uint16_t keyRecorded);
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
    // Wake up on exit from ISR
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 31);
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    __enable_irq();
}

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
    typedef enum mode
    {
        weigh, weighInit, units, heightFtInit, heightFt, heightInInit, heightIn, zero, cal
    } mode_t;

    uint16_t* keyRecorded = 0;
    uint32_t tmp = 0;
    float tmp_f = 0;
    mode_t currentMode = weigh;
    init();
    updateScale();
    while (1)
    {
        //on keypress
        if (checkPress())
        {
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
                    currentMode = heightFtInit;
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
            case heightFt:
                tmp = bitConvertInt(*keyRecorded) * 12;
                currentMode = heightInInit;
                delay_ms(100,sysFreq);
                break;
            default:
                break;
            }
            writeString("s");
        }
        //looping
        switch (currentMode)
        {
        case weighInit:
            updateScale();
            currentMode = weigh;
            break;
        case units:
            updateUnits();
            while (!checkPress())
                ;
            keyRecorded = getKeyArr();
            switch (*keyRecorded)
            {
            case ONE :
                updateSI();
                break;
            case TWO :
                updateImp();
                break;
            default:
                currentMode = units;
                break;
            }
            currentMode = weighInit;
            break;
        case heightFtInit:
            updateHeightFt(); //prompt for entering feet
            currentMode = heightFt;
            break;
        case heightInInit:
            updateHeightIn(); //prompt for entering inches
            currentMode = heightIn;
            break;
        case heightIn:
            while(getArrSize() < 2);
            keyRecorded = getKeyArr();
            tmp+= bitConvertInt(keyRecorded[0])*10;
            tmp += bitConvertInt(keyRecorded[1]);
            changeHeight(tmp);
            currentMode = weighInit;
            delay_ms(100,sysFreq);
            checkPress();
            break;
        case zero:
            tare(10);
            currentMode = weigh;
            break;
        case cal:
            calScreen();
            while(getArrSize() < 2);
            keyRecorded = getKeyArr();
            tmp_f = bitConvertInt(keyRecorded[0]) * 10 + bitConvertInt(keyRecorded[1]);
            writeData('.');
            while(getArrSize() < 2);
            keyRecorded = getKeyArr();
            tmp_f = (float)bitConvertInt(keyRecorded[0]) / 10 + (float)bitConvertInt(keyRecorded[1]) / 100;
            calibrate(tmp_f);
            checkPress();
            currentMode = weighInit;
            delay_ms(100,sysFreq);
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
#ifdef out
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt
    TIMER_A0->CCR[0] = CCR_INCR;//set initial increment
    //set timer to SMCLK, continuous mode, no prescaler
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
    TIMER_A_CTL_MC__CONTINUOUS;
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
        }
        else
        {
            enterSleep = 0;
            timerCounter += 1;
        }
    }
}

