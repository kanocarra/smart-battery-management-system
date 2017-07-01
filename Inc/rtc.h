
/**
  ******************************************************************************
  * File Name          : rtc.h
  * Description        : This file provides code for the configuration
  *                      of the real time clock.
  ******************************************************************************
  */
#ifndef __rtc_H
#define __rtc_H

#include "stm32f3xx_hal.h"
#include "main.h"

extern RTC_HandleTypeDef hrtc;
volatile RTC_TimeTypeDef RTCtime;
volatile RTC_DateTypeDef RTCdate;
extern void _Error_Handler(char *, int);

void MX_RTC_Init(void);

#endif /*__ rtc_H */
