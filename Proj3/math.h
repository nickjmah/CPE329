/*
 * math.h
 *
 *  Created on: May 18, 2018
 *      Author: Nick
 */

#ifndef MATH_H_
#define MATH_H_
#include "stdint.h"
#define MSB 14
/** \brief calculates the sqrt of a number
 *
 *  Assumes that the number is the square of a 14bit
 *
 *  \param val The value to be rooted
 *  \return The root of Val, assuming no larger than 14bit
 *  Squared
 */
uint32_t sqrtDMM(uint32_t val);


#endif /* MATH_H_ */
