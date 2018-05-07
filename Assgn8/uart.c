/*
 * uart.c
 *
 *  Created on: May 7, 2018
 *      Author: Nick
 */
#include "uart.h"
#include "msp.h"
static uint8_t RxBuffer;
static uint8_t RxFlag=0;

void initUART(void)
{
    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 48000000/(16*115200) = 26.0417
    // Fractional portion = 0.0417
    // User's Guide Table 21-4: UCBRSx = 0xAD
    // UCBRFx = int ( (26.0417 - 26)*16) = 0
    EUSCI_A0->BRW = 26;                     // 48000000/16/115200
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt
    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
}
uint8_t readUARTRxFlag(void)
{
    return RxFlag;
}

uint8_t readUART(void)
{
    RxFlag = 0; //clear the uart flag on read
    return RxBuffer;
}

void sendUART(uint8_t* data, size_t size)
{
    int i = 0;
    while(i < size)
    {
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = *data;
        data++;
        i++;
    }
}

void EUSCIA0_IRQHandler(void)
{
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        RxBuffer = EUSCI_A0->RXBUF;
        sendUART(&RxBuffer, 1);
        RxFlag = 1;
    }
}



