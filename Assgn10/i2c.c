/*
 * i2c.c
 *
 *  Created on: May 21, 2018
 *      Author: Jason Zhou
 */

#include "i2c.h"

void initI2C(void)
{
    // Configure GPIO
    P1->OUT &= ~BIT0;                       // Clear P1.0 output latch
    P1->DIR |= BIT0;                        // For LED
    P1->SEL0 |= BIT6 | BIT7;                // I2C pins

    // Enable eUSCIB0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    // Configure USCI_B0 for I2C mode
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // Master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK
    EUSCI_B0->CTLW1 |= EUSCI_B_CTLW1_ASTP_2;// Automatic stop generated
                                            // after EUSCI_B0->TBCNT is reached
    EUSCI_B0->BRW = 30;                     // baudrate = SMCLK / 30 = 100kHz
    EUSCI_B0->TBCNT = 0x0005;               // number of bytes to be received
    EUSCI_B0->I2CSA = 0x0048;               // Slave address
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Release eUSCI from reset

    EUSCI_B0->IE |= EUSCI_A_IE_RXIE |       // Enable receive interrupt
            EUSCI_B_IE_NACKIE |             // Enable NACK interrupt
            EUSCI_B_IE_BCNTIE;              // Enable byte counter interrupt
}
