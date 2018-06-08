/*
 * helper.c
 *
 *  Created on: Jun 6, 2018
 *      Author: Nick
 */

#include "helper.h"

char* itoa(int val)
{
    static char buf[ITOA_BUF_SIZE] = { 0 }; //create a static buffer for string output
    int i = ITOA_INDEX_START;
    buf[i + 1] = '\0';              //forcing termination character in the event
    do
    {
        buf[i] = "0123456789"[(val % 10)];
        val /= 10;
        i--;

    }
    while (val != 0);
    return &buf[i + 1];                    //return index of beginning of string
}

