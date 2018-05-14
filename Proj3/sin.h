/** \file sin.h
 * \brief Function prototypes for the sin lookup table
 *
 * @author Nick Mah
 * @author Jason Zhou
 *
 */

#include "stdint.h"
#ifndef SIN_H_
#define SIN_H_
///The size of the LUT
#define LUT_SIZE 1067

/** \brief a LUT to map masterCount to a sinewave
 *
 *  This function is a simple LUT. The exact values calculated for the LUT are in an excel
 *  file in the project. The function is calculated to be the same size as masterCount. The
 *  exact values create a sinewave with a built-in DC offset and goes between
 *  0V and VCC
 *
 *  \param time The value of masterCount
 *  \return The value to write to the DAC
 */
uint16_t sinUpdate(uint16_t time);

#endif /* SIN_H_ */
