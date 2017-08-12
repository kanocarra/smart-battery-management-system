/**
  ******************************************************************************
  * File Name          : spi.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  */

#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f3xx_hal.h"
#include "main.h"

#define SPI_BUFFER_LENGTH 12

uint8_t SPI_recieve_buffer[SPI_BUFFER_LENGTH];
uint8_t SPI_transmit_buffer[SPI_BUFFER_LENGTH];

void Slave_select(uint8_t Slave);
uint8_t SPI_transmit(uint8_t data);
void Slave_deselect(void);

extern SPI_HandleTypeDef hspi1;

extern void _Error_Handler(char *, int);

void MX_SPI1_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

