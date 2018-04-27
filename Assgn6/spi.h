/*
 * spi.h
 *
 *  Created on: Apr 25, 2018
 *      Author: Nick
 */

#include "stdint.h"
#include "stddef.h"
#include "msp.h"
#include "freq.h"
#ifndef SPI_H_
#define SPI_H_

#define SPI_STRUCT P1
#define SCLK BIT5
#define MOSI BIT6
#define MISO BIT7
#define SPI_MASK BIT5 | BIT6 | BIT7
#define CS_STRUCT P5
#define CS0 BIT0 //port 5

void initSpi(uint16_t baud);
uint32_t clockDivide(uint16_t baud);
void EUSCIB0_IRQHandler(void);
uint32_t checkSPIReadFlag(void);
uint8_t readSPI(void);
void sendData(uint8_t* data, size_t size);//find array len through size/sizeof(*data)
#endif /* SPI_H_ */
