/** \file dco.h
 * \brief Contains function prototypes for DCO
 *
 * This file only contains one function. This function writes everything
 * required to change the frequency of the DCO.
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#ifndef SET_DCO_H_INCLUDE
#define SET_DCO_H_INCLUDE

#include "msp.h"
#include "freq.h"

/** \brief sets the frequency of the DCO
 *
 * This function takes in a single argument, the frequency in kHz, which is
 * defined in freq.h. If the frequency is not one of the predefined macros,
 * the frequency is set the the auto-default 3MHz. The function also changes
 * the voltage levels to enable 48MHz operation. In order to maintain consistent
 * timings at all frequencies, the voltage levels is set for all frequencies.
 *
 * \return void
 * \param freq The desired set frequency
 */
void set_DCO(uint32_t freq);

#endif /* SET_DCO_H_ */
