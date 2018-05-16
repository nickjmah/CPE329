/*
 * dmm.h
 *
 *  Created on: May 16, 2018
 *      Author: Jason Zhou
 */

#ifndef DMM_H_
#define DMM_H_

#include <stdint.h>
#include "timer.h"

#define DMM_STRUCT P2
#define DMM_0 BIT4

void initFreqMeas(void);
uint16_t readFreqFlag(void);
uint32_t readPeriod(void);
void TA0_N_IRQHandler(void);



#endif /* DMM_H_ */
