/*
 * dmm.c
 *
 *  Created on: May 16, 2018
 *      Author: Jason Zhou
 */

#include "dmm.h"
#include "msp.h"

static uint32_t captureValue[2] = { 0 };
static uint16_t captureFlag = 0;
static uint16_t risingFlag = 0;
static uint16_t doneFlag = 0;
static uint32_t PTPArray[2] = { 0 };
static uint32_t overflow = 0;
static uint32_t overflowSave = 0;

void initFreqMeas(void)
{
    DMM_STRUCT->SEL0 |= DMM_0;
    DMM_STRUCT->DIR &= ~DMM_0;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CM_1 | //trigger on rising edge
            TIMER_A_CCTLN_CCIS_0 | //select CCIxA
            TIMER_A_CCTLN_CCIE | //enable interrupt
            TIMER_A_CCTLN_CAP |
            TIMER_A_CCTLN_SCS; //synchronous
    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 | //SMCLK
            TIMER_A_CTL_MC__CONTINUOUS |
            TIMER_A_CTL_CLR |
            TIMER_A_CTL_IE;
    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);
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
    uint32_t temp = captureValue[1] - captureValue[0]
            + TIMER_MAX * overflowSave;
    overflowSave = 0;
    captureFlag = 0;
//    return temp + (48000000/(200 * temp)) + 1;
    return temp;
}

/**takes in a period from the CCR values and
 * returns what the frequency is
 */

uint32_t calcFreq(void)
{
    uint32_t temp = sysFreq * 1000 / readPeriod();
    return temp + temp/200 + 1;
}

void clearDoneFlag(void)
{
    doneFlag = 0;
}

uint32_t averageDC(void)
{
    uint32_t buffer = 0;
    uint8_t size = 0;
    //Configure Timer 0 for interrupts
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = COUNT_20US_48MHZ;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mode
                    TIMER_A_CTL_MC__CONTINUOUS;
    while (!(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG))
    {
        startConv();
        while (!readADCFlag())
        {
            asm("");
            //prevent while loop from being compiled out at higher optimizations
        }
        buffer += readADC();
        size++;
    }
    return buffer / size;
}

uint32_t averageOffset(void)
{
    uint32_t buffer = 0;
    uint32_t size = 0;
    while (risingFlag == 0)
    {
        P1->OUT |= BIT0;

        startConv();
        while (!readADCFlag())
        {
            asm("");
            //prevent while loop from being compiled out at higher optimizations
        }
        buffer += readADC();
        size++;
        P1->OUT &= ~BIT0;
    }
//    sendUARTString(itoa(size));
//    delay_us(10, sysFreq);
    return buffer / size;
//      return readADC();
}

uint32_t * peakToPeak(void)
{
    uint32_t temp = 0;
    while (risingFlag == 0)
    {
        startConv();
        while (!readADCFlag())
        {
            asm("");
            //prevent while loop from being compiled out at higher optimizations
        }
        temp = readADC();
        if (temp < PTPArray[0])
        {
            PTPArray[0] = temp;
        }
        else if (temp > PTPArray[1])
        {
            PTPArray[1] = temp;
        }
    }
    return PTPArray;
}

uint32_t PTPCalc(uint32_t* array)
{
    return array[1] - array[0];
}

//void TA0_0_IRQHandler(void) {
//    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
//    {
//        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
////        timer_count += 1;
//        TIMER_A0->CCR[0] += COUNT_20US_48MHZ;              // Add Offset to TACCR0
//    }
//}

void TA0_N_IRQHandler(void)
{
    static uint32_t captureCount = 0;
    if (TIMER_A0->CTL & TIMER_A_CTL_IFG)
    {
        TIMER_A0->CTL &= ~TIMER_A_CTL_IFG;
        overflow++;
    }
    if (TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG)
    {
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
        captureValue[captureCount] = TIMER_A0->CCR[1];
        captureCount++;
        if (captureCount == 1)
            overflow = 0;
        if(doneFlag == 0)
            risingFlag += 1;
        if (captureCount == 2)
        {
            overflowSave = overflow;
            captureCount = 0;
            captureFlag = 1;
        }

    }

}

