/**
  ******************************************************************************
  * File Name          : spi.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  */
#ifndef __spi_H
#define __spi_H

#include "stm32f3xx_hal.h"
#include "main.h"

//SPI
#define SPI_BUFFER_LENGTH 12

uint8_t SPI_recieve_buffer[SPI_BUFFER_LENGTH];
uint8_t SPI_transmit_buffer[SPI_BUFFER_LENGTH];

extern SPI_HandleTypeDef hspi1;
extern volatile uint32_t globalTick;

extern void _Error_Handler(char *, int);

void MX_SPI1_Init(void);

#endif /*__ spi_H */
