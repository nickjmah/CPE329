/*
 * spi.c
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */

#include "spi.h"

void initSpi(uint16_t baud)
{
    P1->SEL1 &= ~SPI_MASK;
    P1->SEL0 |= SPI_MASK;

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     //reset universal serial commmunication interface
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST|      //reset again?
                      EUSCI_B_CTLW0_MST|        //set MSP as master
                      EUSCI_B_CTLW0_SYNC|       //enable synchronous communication
                      EUSCI_B_CTLW0_CKPL|       //inactive state is low
                      EUSCI_B_CTLW0_UCSSEL_2|   //select SMCLK
                      EUSCI_B_CTLW0_MSB;        //MSB is received/transmitted first
    EUSCI_B0->BRW = clockDivide(baud); //use sysfreq or sysfreq function here
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST; //turn on eUSCI state machine
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE; //enable interrupts to be received
    NVIC->ISER[0] |= 1<<((EUSCIB0_IRQn) & 31);
}

uint32_t clockDivide(uint16_t baud){
    return sysFreq / baud;
}



