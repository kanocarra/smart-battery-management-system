/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#define UART_BUFFER_LENGTH 120
#define RX_BUFFER_LENGTH 5000


extern UART_HandleTypeDef huart3;
uint8_t UART_transmit_buffer[UART_BUFFER_LENGTH];
uint8_t UART_receive_buffer[RX_BUFFER_LENGTH];
void UART_transmit(unsigned char);
void UART_transmit_word(void);
extern void _Error_Handler(char *, int);
void MX_USART3_UART_Init(void);

extern void _Error_Handler(char *, int);


#ifdef __cplusplus
}
#endif
#endif
