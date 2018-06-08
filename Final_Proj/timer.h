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

#define TIMER_MAX (uint32_t)65535

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

///50us count at 12MHz divided by 4
#define COUNT_50US_12MHZ (uint32_t)150

///60s with ACLK divided by 32
#define COUNT_60S_ACLK_32 (uint16_t)61440
#define COUNT_10MS_3MHZ (uint16_t)120000

///100us count at 48MHz DCO frequency
#define COUNT_100US_48MHZ (uint16_t)4800
///1ms count at 48MHz DCO frequency
#define COUNT_1MS_48MHZ (uint16_t)48000
#define COUNT_1S_48MHZ (uint32_t)48000000
#endif /* TIMER_H_ */
