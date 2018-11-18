/*
 * pwm.h
 *
 *  Created on: Oct 17, 2018
 *      Author: Jason Zhou
 */

#ifndef PWM_H_
#define PWM_H_

#include "msp.h"

///Maximum PWM value
#define MAX_VAL 6990

///Minimum PWM value
#define MIN_VAL 1500

///10% duty cycle PWM increment
#define INCR 305

/** \brief Initializes the pins required for servo operation.
 *
 *  Initializes P6.6 and P6.7 for TA2.3 and TA2.4
 *  Also sets PWM duty cycle and period
 *  \return void
 *
 */
void pwmInit(void);



#endif /* PWM_H_ */
