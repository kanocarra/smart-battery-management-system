/**
  ******************************************************************************
  * File Name          : CAN.h
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  */
#ifndef __can_H
#define __can_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

extern CAN_HandleTypeDef hcan;

extern void _Error_Handler(char *, int);

void MX_CAN_Init(void);

#endif /*__ can_H */

