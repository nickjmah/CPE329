/*
 * uart.c
 *
 *  Created on: May 7, 2018
 *      Author: Nick
 */
#include "uart.h"
#include "msp.h"
static uint8_t RxBuffer; //holds the value of whatever is received over UART
static uint8_t RxFlag = 0;
static uint16_t result = 0;

void initUART(void)
{
    // Configure UART pins
    P1->SEL0 |= BIT2 | BIT3;             // set 2-UART pin as secondary function

    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK; // Configure eUSCI clock source for SMCLK
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
    sendUART("\033[H", sizeof("\033[H"));    //return home
    sendUART("\033[2J", sizeof("\033[2J"));    //clear terminal
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
    while (i < size) //loop for size(while loop chosen for speed)
    {
        while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
            ; //wait until buffer is ready
        EUSCI_A0->TXBUF = *data; //send 1 byte
        data++; //increment pointer
        i++; //increment counter
    }
}
void sendUARTString(char* data)
{
    while (*data) //loop until termination character
    {
        //wait until Tx buffer is ready
        while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG))
            ;
        EUSCI_A0->TXBUF = *data;
        data++;
    }
}
//void parseUART(uint8_t data)
//{
//    if(!(data == '\r'))
//    {
//        uint16_t testChar;
//        testChar = data - '0';
//        if(testChar <= 9)
//            result = result*10 + data;
//        else
//            data = 0;
//    }
//    else if (data == '\r')
//        RxFlag = 1;
//    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
//    EUSCI_A0->TXBUF = data;
//}

void clearResult(void)
{
    result = 0;
}

uint16_t readResult(void)
{
    RxFlag = 0; //clear the static interrupt flag
    return result;
}

void EUSCIA0_IRQHandler(void)
{
    //check if receive IRQ occurred
    if (EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG)
    {
        //read buffer
        RxBuffer = EUSCI_A0->RXBUF;
        uint16_t testChar;
        testChar = RxBuffer - '0'; //subtract out ASCII offset
        if (testChar <= 9)
        { //ensure that the key pressed is actually a number
            result = result * 10 + testChar; //if another transmission occurs, do a decimal bitshift
            RxFlag = 1; //raise flag
        }

    }
}

