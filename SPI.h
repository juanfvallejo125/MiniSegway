/*
 * SPI.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Juan Francisco Valle
 */

#ifndef SPI_dot_H_
#define SPI_dot_H_

#include "msp.h"
#include "driverlib.h"

void configSPI();

void SPI_Read(uint8_t byteCount, uint8_t* rx_buffer, uint8_t* tx_buffer);

void SPI_Write(uint8_t byteCount, uint8_t* buffer);




#endif /* SPI_H_ */
