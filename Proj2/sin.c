/*
 * sin.c
 *
 *  Created on: Apr 30, 2018
 *      Author: Nick
 */
#include "sin.h"

uint16_t sinUpdate(uint16_t time)
{
    static const uint16_t lookup[65536] =
    {

    };
    time = lookup[time];
    return time;
}


