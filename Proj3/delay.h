/** \file delay.h
 * \brief Contains function prototypes for delay functionality
 *
 * This file contains two functions. One can delay in microseconds and the
 * other in milliseconds
 *
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#include "stdint.h"

/** \brief compensator for delay_ms()
 *
 * This number is the number of cycles in the while loop
 */
#define CYCLES_MS (uint32_t)4

/** \brief compensator for delay_us()
 *
 * This number is the number of cycles in the while loop multiplied by 1000 to
 * convert units and subtracted by 50 to compensate for a slight amount of
 * slope change
 */
#define CYCLE_US (uint32_t)3950

/** Offset to compensate for the number of cycles needed to enter the function
 *  and init
 */
#define OFFSET_US (uint32_t)25800

/** \brief delay for some amount of ms
 *
 * This function forces the microcontroller to delay for ms at a an operating
 * frequency freq. The function uses some math to determine the amount of
 * loops in a while loop required to delay the exact amount of time. The
 * function uses an assembly directive to force the compiler to not optimize
 * out the loop.
 *
 * \return void
 *
 * \bug This function must be retrimmed every time you change the optimization
 * level. Can be fixed by using a timer instead of a while loop for the delay
 * source.
 */
void delay_ms(uint32_t ms, uint32_t freq);

/** \brief delay for some amount of us
 *
 * This function forces the microcontroller to delay for us at a an operating
 * frequency freq. The function uses some math to determine the amount of loops
 * in a while loop required to delay the exact amount of time. The function uses
 * an assembly directive to force the compiler to not optimize out the loop.
 * The function is nearly identical to delay_ms()
 *
 * \return void
 *
 * \bug This function must be retrimmed every time you change the optimization
 * level. Can be fixed by using a timer instead of a while loop for the delay
 * source.
 *
 * \bug The function is unable to generate small us delays at certain
 * frequencies due to the amount of time it takes to enter the function and init.
 */
void delay_us(uint32_t us, uint32_t freq);

#endif /* DELAY_H_ */
