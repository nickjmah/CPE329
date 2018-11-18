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
static uint8_t ADCFlag = 0;

char* itoa(int val)
{
    static char buf[ITOA_BUF_SIZE] = { 0 }; //create a static buffer for string output
    int i = ITOA_INDEX_START;
    buf[i + 1] = '\0';              //forcing termination character in the event
                                    //of unexpected behaviour
    for (; val != 0; i--)                  //loop until val is 0
    {
        if(i==ITOA_INDEX_START - 2)
        {
            buf[i] = '.';
        }
        else
        {
            buf[i] = "0123456789"[(val) % 10]; //give buf the least significant bit of val
            val /= 10;                        //decimal shift right val

        }

    }
    return &buf[i + 1];                    //return index of beginning of string
}

void initADC(void)
{
    ADC_STRUCT->SEL1 |= ADC_0;                       // Configure P5.4 for ADC
    ADC_STRUCT->SEL0 |= ADC_0;
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);       //attach interrupt

    // Sampling time, S&H=4, ADC14 on, multiple sample and conversion
    ADC14->CTL0 = ADC14_CTL0_SHT0_0 | ADC14_CTL0_SHP | ADC14_CTL0_ON
            | ADC14_CTL0_MSC;
    ADC14->CTL1 = ADC14_CTL1_RES__14BIT; // Use sampling timer, 14-bit conversion results

    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;   // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0;         // Enable ADC conv complete interrupt
}

uint16_t readADCFlag(void)
{
    return ADCFlag;
}

uint32_t readADC(void)
{
    ADCFlag = 0;
    return ADCMem0 + ADC_CAL; //adds calibration value to make results more accurate
}

void startConv(void)
{
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;        //enables a conversion
}

void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0)    // check if MEM0 flag is raised
    {
        ADCMem0 = ADC14->MEM[0]; //save the MEM[0] register, which is a single result
        ADCFlag = 1;            //raise ADC read flag
    }
}

