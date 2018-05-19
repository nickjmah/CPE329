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
/** \brief Initializes UART for terminal
 *  Sets up UART to run at 115200 baud and clears the terminal
 */
void initUART(void);
/** \brief IRQ for UART Receive
 *
 * This function will only accept a number. Upon receive, a static value will be updated to hold
 * the integer representation of the string using a custom atoi
 */
void EUSCIA0_IRQHandler(void);
/** \brief Checks if an interrupt occurred
 *
 *
 */
uint8_t readUARTRxFlag(void);
/** \brief Reads data from the Rx buffer
 *
 *  Unused function
 */
uint8_t readUART(void);
/** \brief sends an arbitrary amount of data over the UART bus
 *
 * Sends an arbitrary amount of chars to the uart buffer. Use size to determine the sent size.
 * \param data a pointer to the array of data to be sent
 * \param size the amount of data bytes to be sent
 */
void sendUART(uint8_t* data, size_t size);
/** \brief Sends a string over the UART bus
 *  This function takes in a string and will print the entire contents of the string
 */
void sendUARTString(char* data);
//void parseUART(uint8_t data);
/** \brief clears the receive value
 *
 */
void clearResult(void);
/** \brief returns the value of the static receive value
 *
 */
uint16_t readResult(void);
#endif /* UART_H_ */
