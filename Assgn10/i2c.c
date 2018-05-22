/*
 * i2c.c
 *
 *  Created on: May 21, 2018
 *      Author: Jason Zhou
 */

#include "i2c.h"

static uint8_t RXData;

void initI2C(void)
{
    // Configure GPIO
    P1->SEL0 |= BIT6 | BIT7;                // I2C pins

    // Enable eUSCIB0 interrupt in NVIC module
//    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    // Configure USCI_B0 for I2C mode
    EUSCI_B0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Software reset enabled
    EUSCI_B0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset mode
            EUSCI_B_CTLW0_MODE_3 |          // I2C mode
            EUSCI_B_CTLW0_MST |             // Master mode
            EUSCI_B_CTLW0_SYNC |            // Sync mode
            EUSCI_B_CTLW0_SSEL__SMCLK;      // SMCLK
    EUSCI_B0->CTLW1 |= EUSCI_B_CTLW1_ASTP_2;// Automatic stop generated
                                            // after EUSCI_B0->TBCNT is reached
    EUSCI_B0->BRW = BAUD_DIV_400KHZ;        // baudrate = SMCLK(48MHz) /120 = 400kHz
    EUSCI_B0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Release eUSCI from reset

    EUSCI_B0->IE |= EUSCI_B_IE_TXIE0 |      // Enable transmit interrupt
            EUSCI_B_IE_NACKIE;              // Enable NACK interrupt
}

void sendI2C(uint8_t address,uint8_t* payload, size_t size)
{
    // configure slave address
    EUSCI_B0->I2CSA = address;
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR | // I2C TX
            EUSCI_B_CTLW0_TXSTT;        // Start condition
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B0->TXBUF = *payload;
    // I2C stop condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP; //not sure if this should be here, it was in the interrupt
    // Clear USCI_B0 TX int flag
    EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG; //ditto here
}

uint8_t readI2C(uint8_t address, uint8_t* RxData_Ptr)
{
    // configure slave address
    EUSCI_B0->I2CSA = (uint32_t)address;
    // I2C start condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0));
    RXData = EUSCI_B0->RXBUF;
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
    return RXData;
}

// I2C interrupt service routine
//void EUSCIB0_IRQHandler(void)
//{
//    if (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)
//    {
//        EUSCI_B0->IFG &= ~EUSCI_B_IFG_NACKIFG;
//
//        // I2C start condition
//        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
//    }
//    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)
//    {
//        EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0;
//        // I2C stop condition
//        TXSendFlag = 1;
//    }
//    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)
//    {
//        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG0;
//        // I2C stop condition
//        RXReceiveFlag = 1;
//    }
//}

