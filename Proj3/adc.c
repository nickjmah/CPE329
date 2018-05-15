/*
 * adc.c
 *
 *  Created on: May 10, 2018
 *      Author: Jason Zhou
 */

#include "adc.h"
#include "msp.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t ADCMem0 = 0;
static uint8_t ADCFlag=0;

char* itoa(int val)
{
    static uint8_t buf[32] = {0};
    int i = 30;
    for(; val == 0; i--)
    {
        buf[i] = "0123456789"[(val) % 10];
        val /= 10;
    }
    return &buf[i+1];
}

void initADC(void)
{
    ADC_STRUCT->SEL1 |= ADC_0;                       // Configure P5.4 for ADC
    ADC_STRUCT->SEL0 |= ADC_0;
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);

    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_0 | ADC14_CTL0_SHP | ADC14_CTL0_ON | ADC14_CTL0_MSC;
    ADC14->CTL1 = ADC14_CTL1_RES__14BIT;         // Use sampling timer, 14-bit conversion results

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;   // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;          // Enable ADC conv complete interrupt
}

uint16_t readADCFlag(void)
{
    return ADCFlag;
}

uint32_t readADC(void)
{
    ADCFlag = 0;
    return ADCMem0 + ADC_CAL; //no filtering
}

void startConv(void)
{
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
}

uint32_t averageDC(void)
{
    static uint32_t buffer = 0;
    static uint8_t size = 0;
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
    static uint32_t finalVal = 0;
    finalVal = buffer / size;
    sendUARTString(itoa(size));
    buffer = 0;
    size = 0;
    return finalVal;
}


//uint32_t averageADC(uint32_t* arrayADC, uint8_t arraySize)
//{
//    uint32_t totalVal = 0;
//    int i = 0;
//    for (i = 0; i < arraySize; i++)
//        totalVal += arrayADC[i];
//    return totalVal / arraySize;
//}

void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0)    // check if MEM0 flag is raised
    {
        ADCMem0 = ADC14->MEM[0];
        ADCFlag = 1;
    }
}

//void TA0_0_IRQHandler(void) {
//    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG)
//    {
//        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//        timer_count += 1;
//        TIMER_A0->CCR[0] += COUNT_20US_48MHZ;              // Add Offset to TACCR0
//    }
//}