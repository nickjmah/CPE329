/** \file dco.c
 * \brief Enables changing of the DCO
 *
 * This file contains one function that can change the frequency to be one of any of the
 * defined frequencies in freq.h
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
#include "dco.h"

void set_DCO(uint32_t freq)
{
    CS->KEY = CS_KEY_VAL; // unlock CS registers
    CS->CTL0 = 0; // clear register CTL0

    /* Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
       PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    /* Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
     ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;

    switch(freq){

        case FREQ_1500_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_0; // set DCO = 1.5 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;

        case FREQ_3000_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_1; // set DCO = 3 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;

        case FREQ_6000_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_2; // set DCO = 6 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;

        case FREQ_12000_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_3; // set DCO = 12 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;

        case FREQ_24000_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_4; // set DCO = 24 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;

        case FREQ_48000_KHZ :
            CS->CTL0 = CS_CTL0_DCORSEL_5; // set DCO = 48 MHz
            /* Select MCLK = DCO, no divider */
            CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
             CS_CTL1_SELM_3;
            break;

        default :
            CS->CTL0 = CS_CTL0_DCORSEL_1; // set DCO = 3 MHz
            CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
            // select clock sources
            break;
    }

    CS->KEY = 0; // lock the CS registers
    return;
}



