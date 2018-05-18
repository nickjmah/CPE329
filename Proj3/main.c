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
#define ADC_MAX 16383
//#define SIG_FIGS 3
uint32_t sysFreq = FREQ_48000_KHZ;
uint8_t* itoaForADC(int val)
{
    int adc = val * 330 / ADC_MAX;
    static uint8_t buf[32] = { 0 };
    int i = 30;
    int j = 0;
    for (j = 0; j < SIG_FIGS + 1; i--, j++)
    {
        if (j == SIG_FIGS - 1)
        {
            buf[i] = '.';
        }
        else
        {
            buf[i] = "0123456789"[(adc) % 10];
            adc /= 10;
        }
    }
    return &buf[i + 1];
}

void init(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    set_DCO(sysFreq);
    initUART();
    initADC();
    initGUI();
    initFreqMeas();
}

void main(void)
{
//    uint32_t ADC_Val;
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
    init();
    __enable_irq();

    uint16_t receive = 0;

    while (1)
    {
        uint32_t offset = 0;
        uint32_t vpp = 0;
        uint32_t rms = 0;
        uint32_t prev;
        uint32_t ACVals[3] = { 0 };
        uint32_t* ACVal_ptr = &(ACVals[0]);
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

//        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
//        __disable_irq();

//        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;
//        __enable_irq();

    }
}

