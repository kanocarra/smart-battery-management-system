/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
*/
#ifndef __usart_H
#define __usart_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

#define UART_BUFFER_LENGTH 120
extern UART_HandleTypeDef huart3;
uint8_t UART_transmit_buffer[UART_BUFFER_LENGTH];
uint8_t UART_receive_buffer[UART_BUFFER_LENGTH];
void UART_transmit(unsigned char);
void UART_transmit_word(void);
extern void _Error_Handler(char *, int);
void MX_USART3_UART_Init(void);
#endif /*__ usart_H */

