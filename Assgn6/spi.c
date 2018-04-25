/*
 * spi.c
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */
#include "spi.h"

void initSpi(uint8_t* sclk, uint8_t* mosi, uint8_t* miso, uint32_t cpol, uint32_t cph){
    P1->SEL1 &= ~SPI_MASK;
    P1->SEL0 |= SPI_MASK;

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; //reset universal serial commmunication interface
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST|EUSCI_B_CTLW0_MST|EUSCI_B_CTLW0_SYNC|
                      EUSCI_B_CTLW0_CPL|EUSCI_B_CTLW0_UCSSEL_2|EUSCI_B_CTLW0_MSB;
    EUSCI_B0->BRW = ;//use sysfreq or sysfreq function here
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST; //turn on eUSCI state machine
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE; //enable interrupts to be received
    NVIC->ISER[0] |= 1<<((EUSCI_B0_IRQn) & 31);

}





