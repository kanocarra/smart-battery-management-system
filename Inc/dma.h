/**
  ******************************************************************************
  * File Name          : dma.h
  * Description        : This file contains all the function prototypes for
  *                      the dma.c file
  ******************************************************************************
  ** This notice applies to any and all portions of this file
*/
#ifndef __dma_H
#define __dma_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

/* DMA memory to memory transfer handles -------------------------------------*/
extern void _Error_Handler(char*, int);

void MX_DMA_Init(void);

#endif /* __dma_H */
