/*
 * vt100.h
 *
 *  Created on: May 14, 2018
 *      Author: Nick
 */
#include "stdint.h"
#include "stdio.h"
#include "uart.h"
#define BAR_HEIGHT 10 //bar height in terms of characters
#define BUFFER_SIZE 100
#ifndef VT100_H_
#define VT100_H_

/** \brief draws a barGraph at a spot on the VT100 terminal
 *
 *  \param minVal   The minimum value of the input data
 *  \param maxVal   The maximum value of the input data
 *  \param val      The input data
 *  \param xPos     The x-position of the bar graph
 *  \param yPos     The y-position of the bar graph
 *  \param title    The title for the graph
 *  \return pointer to the buffer holding the string
 */
char* barGraph(uint32_t minVal, uint32_t maxVal, uint32_t val,
               uint32_t xPos, uint32_t yPos, char* title);

#endif /* VT100_H_ */
