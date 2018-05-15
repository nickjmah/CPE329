/*
 * vt100.c
 *
 *  Created on: May 14, 2018
 *      Author: Nick
 */

#include "vt100.h"
char* barGraph(uint32_t minVal, uint32_t maxVal, uint32_t val, uint32_t xPos, uint32_t yPos, char* title)
{
    static char buf[BUFFER_SIZE]; //the buffer that to generate the string
    uint32_t fillAmt = (MAX_PERCENT*val)/(maxVal)-minVal;
    int n=0;
    int i=0;
    char barVal = '-';
    n = sprintf(buf,"\033[%d;%dH %s", yPos+1, xPos, title);
    sendUARTString(buf);
    n = sprintf(buf,"\033[%d;%dH %s", yPos, xPos-3, "0%");
    sendUARTString(buf);
    n = sprintf(buf,"\033[%d;%dH %d%%", yPos-BAR_HEIGHT+1, xPos-5, MAX_PERCENT);
    sendUARTString(buf);
    for(i=0;i<BAR_HEIGHT;i++)
    {
        fillAmt -=BAR_RES;
        if(fillAmt<MAX_PERCENT && fillAmt>=0)
        {
            barVal = '#';

        }
        else
            barVal = '-';
        n = sprintf(buf,"\033[%d;%dH %c %d", yPos-i, xPos, barVal,fillAmt);
        if(n<BUFFER_SIZE)
            sendUARTString(buf);
        else
            sendUARTString("Uh Oh");
    }

    return buf;
}
