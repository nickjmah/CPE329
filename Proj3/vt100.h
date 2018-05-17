/*
 * vt100.h
 *
 *  Created on: May 14, 2018
 *      Author: Nick
 */
#include "stdint.h"
#include "stdio.h"
#include "uart.h"
#include "adc.h"

#ifndef VT100_H_
#define VT100_H_

#define BAR_HEIGHT 10 //bar height in terms of characters
#define BUFFER_SIZE 100
#define MEAS_BUFFER_SIZE 10
#define MAX_PERCENT 100
#define BAR_RES MAX_PERCENT / BAR_HEIGHT;
#define DMM_MAX_VAL 14895 // 3 / 3.3 * 2^14
#define DMM_MIN_VAL 0
#define V_RMS_Y_POS 13
#define V_RMS_X_POS 52
#define V_DC_Y_POS 13
#define V_DC_X_POS 35
#define MEAS_START_XPNT 17
#define MEAS_START_YPNT 4
#define VAC_YPOS 1
#define FREQ_YPOS 2
#define PK_PK_YPOS 3
#define RMS_YPOS 4
#define SIG_FIGS 4
/** \brief initalizes header for the GUI
 *
 */
void initGUI(void);
/** \brief updates GUI
 *
 */
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
char* barGraph(uint32_t val, uint32_t xPos, uint32_t yPos, char* title);
void updateVDC(uint32_t val);
void updateVAC(uint32_t val);
void updateFreq(uint32_t val);
void updatePkPk(uint32_t val);
void updateRMS(uint32_t val);
void updateAll(uint32_t vdc, uint32_t vac, uint32_t freq, uint32_t pkPk,
               uint32_t rms);
/** \converts an ADC value to a number in decimal
 *
 */
char* itoaADC(uint32_t val);
#endif /* VT100_H_ */
