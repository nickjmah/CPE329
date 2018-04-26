/*
 * spi.h
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */

#include "stdint.h"
#include "msp.h"
#include "freq.h"
#ifndef SPI_H_
#define SPI_H_

#define SPI_STRUCT P1
#define SCLK BIT5
#define MOSI BIT6
#define MISO BIT7
#define SPI_MASK BIT5 | BIT6 | BIT7

void initSpi0(uint16_t baud);
uint32_t clockDivide(uint16_t baud);
void EUSCI_B0_IRQHandler(void);
void sendData();

#endif /* SPI_H_ */
