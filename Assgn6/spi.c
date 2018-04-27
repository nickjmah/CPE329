/*
 * spi.c
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */

#include "spi.h"

volatile uint8_t RXDATA;

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

void sendData(uint8_t* data, size_t size){
    int i=0;
    for(i = 0; i < size/sizeof(uint8_t); i++){
        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){
            EUSCI_B0->TXBUF = data[i];
            break;
        }
    }
}

void EUSCI_B0_IRQHandler(void){
    if(checkSPIReadFlag()){
        RXDATA = EUSCI_B0->RXBUF;
    }
}

uint8_t readSPI(void){
    return RXDATA;
}

uint32_t checkSPIReadFlag(void){
    return EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG;
}

