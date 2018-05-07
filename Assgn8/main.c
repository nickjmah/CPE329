#include "msp.h"
#include "dco.h"
#include "freq.h"


/**
 * main.c
 */

///Setting global MCLK frequency
uint32_t sysFreq = FREQ_3000_KHZ;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(sysFreq);
	EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
	EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |
	                    EUSCI_A_CTLW0_SSEL__SMCLK;
	EUSCI_A0->BRW = 1;
	EUSCI_A0->MCTLW = 10<<EUSCI_A_MCTLW_BRF_OFS |
	                    EUSCI_A_MCTLW_OS16;
	NVIC->ISER[0] = 1<<((EUSCIA0_IRQn)&31);
	__enable_irq();

}

void EUSCI_A0_IRQHandler(void) {
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
    }
}
