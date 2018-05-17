/*
 * vt100.c
 *
 *  Created on: May 14, 2018
 *      Author: Nick
 */

#include "vt100.h"

void initGUI(void)
{
    sendUARTString(
            "+--------------------------------------------------------+\r\n"
            "|                       CPE329: DMM                      |\r\n"
            "+--------------------------------------------------------+\r\n"
            "|Voltage  (VDC):                                         |\r\n"
            "|Voltage  (VAC):                                         |\r\n"
            "|Frequency (Hz):                                         |\r\n"
            "|Peak-Peak  (V):                                         |\r\n"
            "|RMS     (Vrms):                                         |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "+--------------------------------------------------------+");
    //init bar graphs as empty
    barGraph(0, V_RMS_X_POS, V_RMS_Y_POS, "Vrms");
    barGraph(0, V_DC_X_POS, V_DC_Y_POS, "Vdc");
    updateAll(DMM_MIN_VAL, DMM_MIN_VAL, DMM_MIN_VAL, DMM_MIN_VAL, DMM_MIN_VAL);
    //draw title
}
void updateVDC(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT, MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf);
    barGraph(val,V_DC_X_POS,V_DC_Y_POS,"Vdc");

}

void updateVAC(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT + VAC_YPOS,
    MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf);
}

void updateFreq(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT + FREQ_YPOS,
    MEAS_START_XPNT,
            itoaADC(val));//TODO: change when freq format decided
    sendUARTString(measBuf);
}

void updatePkPk(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT + PK_PK_YPOS,
    MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf);
}

void updateRMS(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT + RMS_YPOS,
    MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf);
    barGraph(val,V_RMS_X_POS, V_RMS_Y_POS,"Vrms");
}
void updateAll(uint32_t vdc, uint32_t vac, uint32_t freq, uint32_t pkPk,
               uint32_t rms)
{
    updateVDC(vdc);
    updateVAC(vac);
    updateFreq(freq);
    updatePkPk(pkPk);
    updateRMS(rms);
}
char* barGraph(uint32_t val, uint32_t xPos, uint32_t yPos, char* title)
{
    static char buf[BUFFER_SIZE]; //the buffer that to generate the string
    uint32_t fillAmt = (MAX_PERCENT * val) / (DMM_MAX_VAL) - DMM_MIN_VAL;
    int n = 0;
    int i = 0;
    char barVal = '-';
    n = sprintf(buf, "\033[%d;%dH %s", yPos + 1, xPos, title);
    sendUARTString(buf);
    n = sprintf(buf, "\033[%d;%dH 0V", yPos, xPos - 3);
    sendUARTString(buf);
    n = sprintf(buf, "\033[%d;%dH 3V", yPos - BAR_HEIGHT + 1, xPos - 3);
    sendUARTString(buf);
    for (i = 0; i < BAR_HEIGHT; i++)
    {
        fillAmt -= BAR_RES
        ;
        if (fillAmt < MAX_PERCENT)
        {
            barVal = '#';

        }
        else
            barVal = '-';
        n = sprintf(buf, "\033[%d;%dH %c", yPos - i, xPos, barVal);
        if (n < BUFFER_SIZE)
            sendUARTString(buf);
        else
            sendUARTString("Uh Oh");
    }

    return buf;
}

char* itoaADC(uint32_t val)
{
    int adc = val * 33 / 10;
    static char buf[32] = { 0 };
    int i, j = 0;
    for (i = 0; i < SIG_FIGS - 1; i++)
    {
        adc *= 10;
    }
    adc /= ADC_MAX_VAL;
    i = 30;
    buf[i] = 'V';
    i--;
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
