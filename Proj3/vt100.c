/*
 * vt100.c
 *
 *  Created on: May 14, 2018
 *      Author: Nick
 */

#include "vt100.h"

void initGUI(void)
{
    //set initial display mode
    displayAC();
    //init bar graphs as empty
    barGraph(0, V_RMS_X_POS, V_RMS_Y_POS, "Vrms");
    barGraph(0, V_DC_X_POS, V_DC_Y_POS, "Vdc");
    updateAC(DMM_MIN_VAL, 0, DMM_MIN_VAL, DMM_MIN_VAL, "Unknown");
}
void displayDC(void)
{
    sendUARTString(
            "\033[H"
            "+--------------------------------------------------------+\r\n"
            "|                       CPE329: DC Mode                  |\r\n"
            "+--------------------------------------------------------+\r\n"
            "|Voltage  (VDC):                                         |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "| Press NUM key to                                       |\r\n"
            "| Enter AC Mode                                          |\r\n"
            "+--------------------------------------------------------+");
}
void displayAC(void)
{
    sendUARTString(
            "\033[H"
            "+--------------------------------------------------------+\r\n"
            "|                       CPE329: AC Mode                  |\r\n"
            "+--------------------------------------------------------+\r\n"
            "|Voltage  (VAC):                                         |\r\n"
            "|Frequency (Hz):                                         |\r\n"
            "|Peak-Peak  (V):                                         |\r\n"
            "|RMS     (Vrms):                                         |\r\n"
            "|Waveform      :                                         |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "|                                                        |\r\n"
            "| Press 1 to                                             |\r\n"
            "| Enter DC Mode                                          |\r\n"
            "+--------------------------------------------------------+");
    updateAC(DMM_MIN_VAL, 0, DMM_MIN_VAL, DMM_MIN_VAL, "Unknown");
}
void updateVDC(uint32_t val)
{
    //\033[ROW;HEIGHT
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    //set cursor to start position and print voltage determine by itoaADC
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT, MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf); //print the buf
    barGraph(val, V_DC_X_POS, V_DC_Y_POS, "Vdc"); //update the bargraph

}

void updateVAC(uint32_t val)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    sprintf(measBuf, "\033[%d;%dH %s", MEAS_START_YPNT + VAC_YPOS,
    MEAS_START_XPNT,
            itoaADC(val));
    sendUARTString(measBuf);
    barGraph(val, V_DC_X_POS, V_DC_Y_POS, "Vdc"); //update the bargraph

}

void updateFreq(uint32_t val)
{
//    clearMeas(FREQ_YPOS);
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    //appending units and clearing excess numbers
    sprintf(measBuf, "\033[%d;%dH %sHz    ", MEAS_START_YPNT + FREQ_YPOS,
    MEAS_START_XPNT,
            itoa(val)); //TODO: change when freq format decided
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
    barGraph(val, V_RMS_X_POS, V_RMS_Y_POS, "Vrms");
}
void updateWave(char* wave)
{
    static char measBuf[MEAS_BUFFER_SIZE];
    //write spaces to clear information at end incase of shorter message
    sprintf(measBuf, "\033[%d;%dH %s        ", MEAS_START_YPNT + WAVE_YPOS,
    MEAS_START_XPNT,
            wave);
    sendUARTString(measBuf);
}
void updateAC(uint32_t vac, uint32_t freq, uint32_t pkPk, uint32_t rms,
              char* wave)
{
    updateVAC(vac);
    updateFreq(freq);
    updatePkPk(pkPk);
    updateRMS(rms);
    updateWave(wave);
}
char* barGraph(uint32_t val, uint32_t xPos, uint32_t yPos, char* title)
{
    int n, i = 0;
    char barVal = '-'; //set barVal initial value to empty
    static char buf[BUFFER_SIZE]; //the buffer that to generate the string
    uint32_t fillAmt = (MAX_PERCENT * val) / (DMM_MAX_VAL) - DMM_MIN_VAL;
    //fillAmt maps val to a percentage from 0 to 100

    /* format and print title and side bars
     * First print the title at the bottom of the graph, then the min value, then the max
     */
    n = sprintf(buf, "\033[%d;%dH %s \033[%d;%dH 0V \033[%d;%dH 3V", yPos + 1,
                xPos, title, yPos, xPos - 3, yPos - BAR_HEIGHT + 1, xPos - 3);
    sendUARTString(buf); //send string

    //begin bar value
    for (i = 0; i < BAR_HEIGHT; i++)
    {
        fillAmt -= BAR_RES; //subtracts fillAmt by resolution for each loop
        if (fillAmt < MAX_PERCENT) //check to make sure that there is remaining perc. to fill out
        {
            barVal = '#'; //if so, fill in the next block with a full character

        }
        else
            barVal = '-'; //otherwise empty
        n = sprintf(buf, "\033[%d;%dH %c", yPos - i, xPos, barVal); //format result
        if (n < BUFFER_SIZE) //check for errors
            sendUARTString(buf);
        else
            sendUARTString("Uh Oh");
    }

    return buf;
}

char* itoaADC(uint32_t val)
{ //TODO: remove magic numbers
    static char buf[ITOA_BUF_SIZE] = { 0 }; //create an output string buffer
    int i, j = 0;
    i = ITOA_INDEX_START;
    //convert val to ADC voltage in fixed point precision
    int adc = val * ADC_MAX_ANALOG_VAL / UNITS_MILLI; //multiply by 3.3V
    for (i = 0; i < SIG_FIGS - 1; i++) //decimal shift left for SIG_FIGS precision
    {
        adc *= 10;
    }
    adc /= ADC_MAX_VAL; //final division to finish conversion
    //generate buffer string
    buf[i] = 'V';    //append units
    i--;
    for (j = 0; j < SIG_FIGS + 1; i--, j++)
    {
        if (j == SIG_FIGS - 1) //place decimal point
        {
            buf[i] = '.';
        }
        else
        {
            buf[i] = "0123456789"[(adc) % 10]; //indexing least significant decimal
            adc /= 10; //decimal shift right
        }
    }
    return &buf[i + 1]; //send out string indexed at first character
}
void clearMeas(uint32_t yPos)
{
    static char measBuf[MEAS_BUFFER_SIZE]; //the buffer that to generate the string
    //write nothing to overwrte all of the characters at te given ypos for a measurement
    sprintf(measBuf, "\033[%d;%dH       ", MEAS_START_YPNT + yPos,
    MEAS_START_XPNT);
    sendUARTString(measBuf);
    //clearing 6 spaces to remove any lingering values
    return;
}
