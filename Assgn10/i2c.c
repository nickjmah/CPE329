/*
 * i2c.c
 *
 *  Created on: May 21, 2018
 *      Author: Jason Zhou
 */

#include "i2c.h"

static uint8_t RXData[5] = {0};
static uint8_t RXDataPointer;
static uint8_t RXReceiveFlag = 0;

// Pointer to TX data
static uint8_t TXData[]= {0xA1, 0xB1, 0xC1, 0xD1};

static uint8_t TXByteCtr;
static uint8_t SlaveFlag = 0;

void initI2C(void)
{
    // Configure GPIO
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
    // Ensure stop condition got sent
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    EUSCI_B0->TXBUF = *payload;
}

uint8_t readI2C(uint8_t address, uint8_t* RxData_Ptr)
{
    // configure slave address
    EUSCI_B0->I2CSA = (uint32_t)address;

    // I2C start condition
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    // Ensure stop condition got sent
    while (EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP);
    uint16_t bytes = EUSCI_B0->STATW;
    return (uint8_t)((bytes & 0xFF00) >> 8);
}

uint8_t readI2CReceiveFlag(void)
{
    return RXReceiveFlag;
}

// I2C interrupt service routine
void EUSCIB0_IRQHandler(void)
{
    if (EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_NACKIFG;

        // I2C start condition
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)
    {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG0;

        // Check TX byte counter
        if (TXByteCtr)
        {
            // Load TX buffer
            EUSCI_B0->TXBUF = TXData[SlaveFlag];

            // Decrement TX byte counter
            TXByteCtr--;
        }
        else
        {
            // I2C stop condition
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

            // Clear USCI_B0 TX int flag
            EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG;
        }
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_RXIFG0;
        // I2C stop condition
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
        RXReceiveFlag = 1;

        // Get RX data
        RXData[RXDataPointer++] = EUSCI_B0->RXBUF;

        if (RXDataPointer > sizeof(RXData))
        {
            RXDataPointer = 0;
        }
    }
    if (EUSCI_B0->IFG & EUSCI_B_IFG_BCNTIFG)
    {
        EUSCI_B0->IFG &= ~ EUSCI_B_IFG_BCNTIFG;
    }
}

