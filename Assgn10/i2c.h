/*
 * i2c.h
 *
 *  Created on: May 21, 2018
 *      Author: Jason Zhou
 */

#ifndef I2C_H_
#define I2C_H_

#include "msp.h"
#include "stdint.h"
#include "stdio.h"
void initI2C(void);
void sendI2C(uint8_t address, size_t size);
/** \brief reads RxData
 *
 *  \param RxData_Ptr pointer to the first element of the array containing RxData
 *  \return The size of the amount of data sent
 *
 */
uint8_t readI2C(uint8_t* RxData_Ptr);
uint8_t readI2CReceiveFlag(void);
#endif /* I2C_H_ */