/*
 * dmm.c
 *
 *  Created on: May 16, 2018
 *      Author: Jason Zhou
 */

#include "dmm.h"
#include "msp.h"

static uint32_t captureValue[2] = {0};
static uint16_t captureFlag = 0;
static uint16_t risingFlag = 0;
static uint16_t doneFlag = 0;

void initFreqMeas(void)
{
    DMM_STRUCT->SEL0 |= DMM_0;
    DMM_STRUCT->DIR &= ~DMM_0;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CM_1| //trigger on rising edge
                        TIMER_A_CCTLN_CCIS_0| //select CCIxA
                        TIMER_A_CCTLN_CCIE| //enable interrupt
                        TIMER_A_CCTLN_CAP|
                        TIMER_A_CCTLN_SCS; //synchronous
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2| //SMCLK
                     TIMER_A_CTL_MC__CONTINUOUS|
                     TIMER_A_CTL_CLR;
    NVIC->ISER[0] = 1<<((TA0_N_IRQn)&31);
}

uint16_t readFreqFlag(void)
{
    return captureFlag;
}

uint16_t readRisingFlag(void)
{
    return risingFlag;
}

void clearRisingFlag(void)
{
    risingFlag = 0;
}

uint32_t readPeriod(void)
{
    captureFlag = 0;
    return captureValue[1] - captureValue[0];
}

void clearDoneFlag(void)
{
    doneFlag = 0;
}

uint32_t averageDC(void)
{
    uint32_t buffer = 0;
    uint8_t size = 0;
    initTimer();
    while(!(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG))
    {
        startConv();
        while(!readADCFlag()){
            asm(""); //prevent while loop from being compiled out at higher optimizations
        }
        buffer += readADC();
        size ++;
    }
    doneFlag = 1;
    return buffer / size;
}

uint32_t averageOffset(void)
{
    uint32_t buffer = 0;
    uint32_t size = 0;
    while(risingFlag == 0)
    {
        P1->OUT |= BIT0;

        startConv();
        while(!readADCFlag()){
            asm(""); //prevent while loop from being compiled out at higher optimizations
        }
        buffer += readADC();
        size ++;
        P1->OUT &= ~BIT0;
    }
//    sendUARTString(itoa(size));
//    delay_us(10, sysFreq);
    return buffer / size;
//      return readADC();
}

void TA0_N_IRQHandler(void)
{
//    static uint32_t captureCount = 0;
    if(TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG){
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
//        captureValue[captureCount] = TIMER_A0->CCR[1];
//        captureCount++;
        if(doneFlag == 0)
            risingFlag += 1;
//        if(captureCount == 2){
//            captureCount = 0;
//            captureFlag = 1;
//        }

    }

}

