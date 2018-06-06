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
volatile uint32_t enterSleep = 0;//TODO: change to enum maybe
volatile uint32_t timerCounter = 0;
volatile uint32_t keyPress = 0;
void sleep(void);
void boardInit(void);
void initSleepTimer(void);
void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    boardInit();//turn off everything to save power
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
    while(1){
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
    init();
    updateScale();
    while(1)
    {
        if(keyPress)
        {
            keyPress = 0;
            writeData(bitConvertChar(checkKP()));
            sleep();
            writeString("s");
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
    P1->OUT = 0x00; P1->DIR = 0xFF;
    P2->OUT = 0x00; P2->DIR = 0xFF;
    P3->OUT = 0x00; P3->DIR = 0xFF;
    P4->OUT = 0x00; P4->DIR = 0xFF;
    P5->OUT = 0x00; P5->DIR = 0xFF;
    P6->OUT = 0x00; P6->DIR = 0xFF;
    P7->OUT = 0x00; P7->DIR = 0xFF;
    P8->OUT = 0x00; P8->DIR = 0xFF;
    P9->OUT = 0x00; P9->DIR = 0xFF;
    P10->OUT = 0x00; P10->DIR = 0xFF;
    PJ->OUT = 0x00; PJ->DIR = 0xFF;
}
void initSleepTimer(void)
{
#ifdef out
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; //enable interrupt
    TIMER_A0->CCR[0] = CCR_INCR;            //set initial increment
    //set timer to SMCLK, continuous mode, no prescaler
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |
            TIMER_A_CTL_MC__CONTINUOUS;
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);//attach interrupt
#endif
}
void TA0_0_IRQHandler(void)
{
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
        TIMER_A0->CCR[0] += CCR_INCR;
        if(timerCounter >= TEN_SECONDS)
        {
            enterSleep=1; //sleep after 10s of inactivity, otherwise, increment counter
        }
        else
        {
            enterSleep=0;
            timerCounter += 1;
        }
    }
}
void PORT4_IRQHandler(void)
{
    if(P4->IFG & (C0|C1|C2))
    {
        P4->IFG &= ~(C0|C1|C2);
        keyPress = 1;
    }
}

