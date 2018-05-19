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
    static char buf[ITOA_BUF_SIZE] = {0};
    int i = ITOA_INDEX_START;
    for(; val != 0; i--)
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
//                  |ADC14_CTL0_SSEL__SMCLK | ADC14_CTL0_DIV_1 | ADC14_CTL0_PDIV__64;
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

void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0)    // check if MEM0 flag is raised
    {
        ADCMem0 = ADC14->MEM[0];
        ADCFlag = 1;
    }
}

