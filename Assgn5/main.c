#include "msp.h"
#include "dco.h"
#include "freq.h"
/** \file main.c
 * \brief Enables Timers
 *
 * \author Nick Mah
 * \author Jason Zhou
 *
 */

#define FREQ FREQ_24000_KHZ

void init(void)
{
    set_DCO(FREQ);
    return;
}
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

}
