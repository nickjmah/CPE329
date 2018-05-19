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
#define BUFFER_SIZE 100 // the size of the buffer used to draw the bar graph
#define MEAS_BUFFER_SIZE 10 //The size of the buffer to create the voltage readout
#define MAX_PERCENT 100 //used for math purposes
#define BAR_RES MAX_PERCENT / BAR_HEIGHT //resolution of the bar graph
#define DMM_MAX_VAL 14895 // 3 / 3.3 * 2^14 maximum expected DAC Val
#define DMM_MIN_VAL 0
//start values of all of the measurements
#define MEAS_START_XPNT 17
#define MEAS_START_YPNT 4
//defining the cursor positions of the measurements
#define V_RMS_Y_POS 13 
#define V_RMS_X_POS 52
#define V_DC_Y_POS 13
#define V_DC_X_POS 35
#define VAC_YPOS 0
#define FREQ_YPOS 1
#define PK_PK_YPOS 2
#define RMS_YPOS 3
#define WAVE_YPOS 4

#define SIG_FIGS 4 //the amount of digits shown in the measurement
/** \brief initalizes header for the GUI
 * sets up the GUI by printing out the "box" with title and initializing the display
 * with all zeros
 * \return void
 */
void initGUI(void);
/** \brief draws a barGraph at a spot on the VT100 terminal
 *  the barGraph takes in a list of parameters and prints out a bargraph at that
 *  position. A title will be printed at the bottom of the graph as well as the
 *  max and min values of 0 and 3V. The bargraph will display '-' for an empty box
 *  and '#' for a full box. The resolution and size of the box is determined by
 *  the macro BAR_HEIGHT indirectly. The bar graph is printed vertically. Upon
 *  completion the bargraph will return the last printed characters. Upon an
 *  error "Uh Oh" will printed at the location of the error
 *  \param minVal   The minimum value of the input data
 *  \param maxVal   The maximum value of the input data
 *  \param val      The input data
 *  \param xPos     The x-position of the bar graph
 *  \param yPos     The y-position of the bar graph
 *  \param title    The title for the graph
 *  \return pointer to the buffer holding the string
 */
char* barGraph(uint32_t val, uint32_t xPos, uint32_t yPos, char* title);
/** \brief Changes the display to DC mode
 *
 */
void displayDC(void);
/** \brief Changes the display to AC mode
 *
 */
void displayAC(void);
/** \brief updates and converts the voltage data to a DC voltage reading
 * \param val The average DC value from the DMM
 * \return void
 */
void updateVDC(uint32_t val);
/** \brief updates and converts the voltage data to a AC voltage reading
 *  This function will update the display VDC measurement and the bar graph
 * \param val The average AC value from the DMM
 * \return void
 */
void updateVAC(uint32_t val);
/** \brief updates and prints the frequency data
 * \param val The frequency read by the DMM
 * \return void
 */
void updateFreq(uint32_t val);
/** \brief updates and converts the Peak-Peak voltage to a voltage reading
 * \param val The peak to peak value from the DMM
 * \return void
 */
void updatePkPk(uint32_t val);
/** \brief Updates Waveform data
 *
 */
void updateWave(char* wave);
/** \brief updates and converts the rms value to a voltage reading
 * This function will update the RMS printed value and the Vrms bar graph
 * \param val The peak to peak value from the DMM
 * \return void
 */

void updateRMS(uint32_t val);
/** \brief updates all of the values and graphs
 * This function calls all of the updateX functions for AC mode
 * \param vac The AC Voltage reading
 * \param freq The frequency measurement
 * \param pkPk The Peak-Peak voltage measurement
 * \param rms The RMS voltage reading
 * \return void
 */
void updateAC(uint32_t vac, uint32_t freq, uint32_t pkPk, uint32_t rms,
              char* wave);
/** \converts an ADC value to a number in decimal
 * This function takes in a value that is assumed to be a 14-bit number from the
 * ADC and converts it to a fixed point float with SIG_FIGS precision. The
 * function is identical to itoa() except that it accounts for a fixed point
 * number and inserts a '.' A static buffer is used to allow for the string to be
 * accumulated
 * \param val a 14-bit number from the ADC
 * \return a pointer to buf indexed to the first digit
 * \bug the function assumes that the value is no greater than 2^14. Unknown operation will occur
 * \bug The adc initialization doesn't take SIG_FIGS into account and must be adjusted every time
 *
 */
char* itoaADC(uint32_t val);
/** \brief clears the measurement value to fix unchanged characters
 *
 *
 */
void clearMeas(uint32_t yPos);

#endif /* VT100_H_ */
