#include "msp.h"
#include "dco.h"
#include "freq.h"
#include "uart.h"
#include "adc.h"
#include "dmm.h"
#include "vt100.h"
#include "math.h"
/**
 * main.c
 */
uint32_t sysFreq = FREQ_48000_KHZ; //set system frequency to 48MHz

void init(void)
{
    //initialize all peripherals
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initUART();
    initADC();
    initGUI();
    initFreqMeas();
}

void main(void)
{
    uint16_t receive = 0; //holds the number a key press on the UART Terminal
    uint32_t offset = 0;
    uint32_t vpp = 0;
    uint32_t rms = 0;
    uint32_t prev;
    uint32_t ACVals[3] = { 0 };
    uint32_t* ACVal_ptr = &(ACVals[0]);
    init();
    __enable_irq();

    while (1)
    {

        if (readUARTRxFlag())
        {
            prev = receive;
            receive = readResult();
            clearResult();     //clear the value of result
            if (prev != receive)//TODO: screen needless refreshes still
            {
                switch (receive)
                {
                case (1):
                    displayDC();
                    break;
                default:
                    displayAC();
                }
            }
        }
        switch (receive)
        {
        case (1):
            updateVDC(averageDC());
            break;
        default:
            if (readFreqFlag())
            {
                updateFreq(calcFreq());
            }
            ACMeas(ACVal_ptr);
            offset = OffsetCalc(ACVals[1], ACVals[2]);
            updateVAC(offset);
            vpp = PTPCalc(ACVals[1], ACVals[2]);
            updatePkPk(vpp);
            rms = sqrtDMM((ACVals[0]));
            updateRMS(rms);
            updateWave(waveDetect(rms, vpp, offset));
        }

    }
}

