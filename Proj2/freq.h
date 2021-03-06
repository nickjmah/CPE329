/** \file freq.h
 * \brief Contains macros for use in defining system frequency
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
#ifndef FREQ_H_INCLUDED
#define FREQ_H_INCLUDED

#include <stdint.h>

///System Frequency
extern uint32_t sysFreq;
///frequency at 1500kHz
#define FREQ_1500_KHZ (uint32_t)1500

///frequency at 300kHz
#define FREQ_3000_KHZ (uint32_t)3000

///frequency at 6000kHz
#define FREQ_6000_KHZ (uint32_t)6000

///frequency at 12000kHz
#define FREQ_12000_KHZ (uint32_t)12000

///frequency at 24000kHz
#define FREQ_24000_KHZ (uint32_t)24000

///frequency at 48000kHz
#define FREQ_48000_KHZ (uint32_t)48000
//test
#endif /* FREQ_H_ */
