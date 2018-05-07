/*
 * uart.h
 *
 *  Created on: May 7, 2018
 *      Author: Nick
 */

#ifndef UART_H_
#define UART_H_
#include "stdint.h"
#include "stdlib.h"
/** \brief Initializes UART
 *
 */
void initUART(void);
/** \brief IRQ for UART Receive
 *
 * This function automatically echos what was sent as well TODO:check if this is nec.
 */
void EUSCIA0_IRQHandler(void);
/** \brief Checks if an interrupt occured
 *
 *
 */
uint8_t readUARTRxFlag(void);
/** \brief Reads data from the Rx buffer
 *
 *  TODO: consider changing this to a pointer
 */
uint8_t readUART(void);
/** \brief sends an arbitrary amount of data over the UART bus
 *
 *
 */
void sendUART(uint8_t* data, size_t size);
void parseUART(uint8_t data);
void clearResult(void);
uint16_t readResult(void);
#endif /* UART_H_ */
