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
    SPI_STRUCT->SEL1 &= ~SPI_MASK; //resetting port 1 select bits
    SPI_STRUCT->SEL0 |= SPI_MASK; //setting port 0 select bits
    CS_STRUCT->DIR |= CS0; //setting chip select direction
    CS_STRUCT->OUT |= CS0; //setting chip select high

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     //reset universal serial
                                                //communication interface
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST|
                      EUSCI_B_CTLW0_MST|        //set MSP as master
                      EUSCI_B_CTLW0_SYNC|       //enable synchronous communication
                      EUSCI_B_CTLW0_CKPL|       //inactive state is low
                      EUSCI_B_CTLW0_UCSSEL_2|   //select SMCLK
                      EUSCI_B_CTLW0_MSB;        //MSB is received/transmitted first
    EUSCI_B0->BRW = clockDivide(baud); //use sysfreq or sysfreq function here
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST; //turn on eUSCI state machine
    EUSCI_B0->IE = EUSCI_B_IE_RXIE; //enable interrupts to be received
    NVIC->ISER[0] = 1<<((EUSCIB0_IRQn) & 31); //enable NVIC interrupts
}

uint32_t clockDivide(uint16_t baud){
    return sysFreq / baud; //setting clock division by dividing system frequency
                           //by desired baud rate
}

void sendData(uint8_t* data, size_t size){
    CS_STRUCT->OUT &= ~CS0; //reset chip select low to start data transmission
    int i=0; //initialize counter to zero
    for(i = 0; i < size; i++){ //loop for size of array
        while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){ //no op for when interrupt flag is
                                                     //not raised
            asm(""); //prevent while loop from being compiled out at higher optimizations
        }
        EUSCI_B0->TXBUF = *data; //set TXbuffer to one byte of data
        data++; //increment data byte pointer
    }
    CS_STRUCT->OUT |= CS0; //set chip select high to end data transmission
}

void EUSCIB0_IRQHandler(void){
    if(checkSPIReadFlag()){ //check SPI read flag
        RXDATA = EUSCI_B0->RXBUF; //set volatile RXDATA variable to contents of RXBUF
                                  //also resets EUSCI_BO RX interrupt flag
    }
}

uint8_t readSPI(void){
    return RXDATA; //read back volatile RXDATA variable
}

uint32_t checkSPIReadFlag(void){
    return EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG; //return 1 or 0 if EUSCI_B0 RX interrupt
                                              //flag was raised
}

