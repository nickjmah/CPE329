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
    int n=0;
    n = sprintf(buf,"\033[%d;%dH %d", yPos, xPos, val);
    if(n<BUFFER_SIZE)
        sendUARTString(buf);
    else
        sendUARTString("Uh Oh");
    return buf;
}
