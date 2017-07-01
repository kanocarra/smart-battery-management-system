/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsm.h"
#include "PEC15.h"

void SystemClock_Config(void);

void main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_CAN_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();

 State currentState = idle;

 	set_UV_OV_threshold();

	globalTick=0; //for counting 1 second in the main loop
	uint8_t loggingCounter = 0; //for adjusting data logging frequency.

  while (1)
  {
    currentState = (State)currentState();
  }

}

State idle(void){
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
  HAL_Delay(1000);
  return (State)start;
}

State start(void){
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 1); 
  HAL_Delay(1000);
  return (State)measure;
}

State measure(void){
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 0);   
  return (State)estimate_soc;
}

State estimate_soc(void){
  HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 0); 
  return (State)compute_resistance;
}

State compute_resistance(void){
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 0); 
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 0); 
  return (State)compute_capacity;
}

State compute_capacity(void){
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 0); 
  HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 0); 
  return (State)balancing;
}

State balancing(void){
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 0); 
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 0); 
  return (State)send_data;

}

State send_data(void){
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 1); 
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 0); 
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 0); 
  return (State)shutdown;

}

State shutdown(void){
  HAL_Delay(1000);
  return (State)idle;
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
