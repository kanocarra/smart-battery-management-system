/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  */
#ifndef __adc_H
#define __adc_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern void _Error_Handler(char *, int);

void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
#endif /*__ adc_H */
