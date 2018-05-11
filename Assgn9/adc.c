/*
 * adc.c
 *
 *  Created on: May 10, 2018
 *      Author: Jason Zhou
 */

#include "adc.h"
#include "msp.h"
static uint32_t ADCMem0 = 0;
static uint8_t ADCFlag=0;

void initADC(void)
{
    ADC_STRUCT->SEL1 |= ADC_0;                       // Configure P5.4 for ADC
    ADC_STRUCT->SEL0 |= ADC_0;
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);

    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES__14BIT;         // Use sampling timer, 14-bit conversion results

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;   // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;          // Enable ADC conv complete interrupt
}

uint16_t readADCFlag(void)
{
    while(ADCFlag == 0){
        asm(""); //prevent while loop from being compiled out at higher optimizations
    }
    return ADCFlag;
}

uint32_t readADC(void)
{
    ADCFlag = 0;
    return ADCMem0 + ADC_CAL; //no filtering
}

void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0)    // check if MEM0 flag is raised
    {
        ADCMem0 = ADC14->MEM[0];
        ADCFlag = 1;
    }
}

