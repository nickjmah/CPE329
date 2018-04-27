/** \file timer.h
 * \brief Contains macros used for various counter increments for Assignment 5
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#include <stdint.h>


#ifndef TIMER_H_
#define TIMER_H_

///20us count or 50% duty cycle at 25kHz at 24MHz DCO frequency
#define COUNT_50_25KHZ_24MHZ (uint32_t)480

///30us count or 75% duty cycle at 25kHz at 24MHz DCO frequency
#define COUNT_75_25KHZ_24MHZ (uint32_t)720

///10us count or 25% duty cycle at 25kHz at 24MHz DCO frequency
#define COUNT_25_25KHZ_24MHZ (uint32_t)240

///100ms count at 1.5MHz DCO frequency divided by 8
#define COUNT_100_1500KHZ (uint32_t)37500

///1ms count at 1.5MHz DCO frequency
#define COUNT_1_1500KHZ (uint32_t)750

///2ms count at 1.5MHz DCO frequency
#define COUNT_2_1500KHZ (uint32_t)1500

///1ms count at 12MHz
#define COUNT_1MS_12MHZ (uint32_t)12000

///20ms count at 12MHz DCO frequency divided by 4
#define COUNT_20MS_12MHZ (uint16_t)30000

#endif /* TIMER_H_ */
