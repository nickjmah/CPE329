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

/** \brief Initializes SPI
 *
 *  Initializes SPI on the #defined pinout. Takes in a baud rate for SPI
 *  that will be used to determine the SPI SCLK divider
 *
 *  \param baud The desired baud rate in KHz
 *  \return void
 */
void initSpi(uint16_t baud);

/** \brief helper function that determines clock division amount
 *
 * \param baud The desired baud rate
 * \return The number required for the prescaler to get the desired SCLK
 * frequency
 */
uint32_t clockDivide(uint16_t baud);

/** \brief IRQ for SPI receive
 *
 *  This IRQ transfers the receive buffer data to a file scope globa
 *
 *  \bug This function will erase any previous data in the buffer upon read.
 *
 */
void EUSCIB0_IRQHandler(void);

/** \brief Checks the SPI receive buffer flag
 *
 * \return The state of the SPI Read Flag
 */
uint32_t checkSPIReadFlag(void);

/** \brief reads the value of the SPI buffer variable
 *
 *  This function is part of encapsulation. It helps functions outside of
 *  the file access the receive buffer
 *
 *  \return uint8_t of the receive buffer's latest data
 *
 */
uint8_t readSPI(void);

/** \brief Sends an arbitrary amount of data over SPI
 *
 * This function takes in an array of chars and sends them over SPI
 * The function takes a pointer to an array as well as the size of
 * the array. The function repeatedly sends SPI data through the Tx
 * data until it has iterated size times.
 *
 * \param data A pointer to the first element in the send array
 * \param size The result of the sizeof() command on data[]
 * \return void
 */
void sendData(uint8_t* data, size_t size);//find array len through size/sizeof(*data)
#endif /* SPI_H_ */
