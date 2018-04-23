#include "msp.h"
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
