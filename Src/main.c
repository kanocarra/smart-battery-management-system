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
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "PEC15.h"
#include "fsm.h"
#include "log.h"

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
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();

  State currentState = idle;

  HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 1); 
  HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 1); 

  while (1)
  {
    currentState = (State)currentState();
  }

}  

State idle(void){
  led_flash(IDLE);
  sprintf(UART_transmit_buffer, "State: Idle");
  UART_transmit_word(); 
  return (State)start;
}

State start(void){

  led_flash(START);
  sprintf(UART_transmit_buffer, "State: Start");
  UART_transmit_word(); 

  //Initialise the battery
  Battery battery = init_battery();
  Battery *const r_battery = &battery;

  // Set under and over voltage thresholds
  set_UV_OV_threshold();

  return (State)measure(r_battery);
}

State measure(Battery *const battery){
  led_flash(MEASURE);
  sprintf(UART_transmit_buffer, "State: Measure");
  UART_transmit_word(); 

  StatusA* status_regA = read_status_A_6804_2();
  sprintf(UART_transmit_buffer, "Temperature: %i %i\n", status_regA->ITMPUB, status_regA->ITMPLB );
  UART_transmit_word(); 

  StatusB* status_regB = read_status_B_6804_2();
  sprintf(UART_transmit_buffer, "Digital Voltage: %i \n", (int)status_regB->VDUB);
  UART_transmit_word(); 

  read_voltage_and_current(battery);
  
  sprintf(UART_transmit_buffer, "Cell 0: %i \n", (int)battery->cells[0].voltage);
  UART_transmit_word(); 
  sprintf(UART_transmit_buffer, "Cell 1: %i \n", (int)battery->cells[1].voltage);
  UART_transmit_word();
  sprintf(UART_transmit_buffer, "Cell 6: %i \n", (int)battery->cells[2].voltage);
  UART_transmit_word(); 
  sprintf(UART_transmit_buffer, "Cell 7: %i \n", (int)battery->cells[3].voltage);
  UART_transmit_word(); 
  sprintf(UART_transmit_buffer, "Current: %i \n", (int)battery->current);
  UART_transmit_word(); 
  
  log_data(battery);

  return (State)estimate_soc(battery);
}

State estimate_soc(Battery *const battery){
  led_flash(ESTIMATE_SOC);
  sprintf(UART_transmit_buffer, "State: Estimate SoC");
  UART_transmit_word(); 
  return (State)compute_resistance;
}

State compute_resistance(void){
  led_flash(COMPUTE_R);
  sprintf(UART_transmit_buffer, "State: Compute Resistance");
  UART_transmit_word(); 
  return (State)compute_capacity;
}

State compute_capacity(void){
  led_flash(COMPUTE_C);
  sprintf(UART_transmit_buffer, "State: Compute Capacity");
  UART_transmit_word(); 
  return (State)balancing;
}

State balancing(void){
  led_flash(BAL);
  sprintf(UART_transmit_buffer, "State: Balancing");
  UART_transmit_word(); 
  return (State)send_data;

}

State send_data(void){
  led_flash(SEND);
  sprintf(UART_transmit_buffer, "State: Send Data");
  UART_transmit_word(); 
  return (State)shutdown;

}

State shutdown(void){
  led_flash(SHUTDOWN);
  sprintf(UART_transmit_buffer, "State: Shutdown");
  UART_transmit_word(); 
  return (State)idle;
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
