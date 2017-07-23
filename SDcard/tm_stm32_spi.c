/**	
 * |----------------------------------------------------------------------
 * | Copyright (c) 2016 Tilen Majerle
 * |  
 * | Permission is hereby granted, free of charge, to any person
 * | obtaining a copy of this software and associated documentation
 * | files (the "Software"), to deal in the Software without restriction,
 * | including without limitation the rights to use, copy, modify, merge,
 * | publish, distribute, sublicense, and/or sell copies of the Software, 
 * | and to permit persons to whom the Software is furnished to do so, 
 * | subject to the following conditions:
 * | 
 * | The above copyright notice and this permission notice shall be
 * | included in all copies or substantial portions of the Software.
 * | 
 * | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * | OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * | AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * | HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * | WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * | OTHER DEALINGS IN THE SOFTWARE.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32_spi.h"

/* Defines for alternate functions */
#define GPIO_AFx_SPI1    GPIO_AF5_SPI1


/* Private functions */
static void TM_SPIx_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit);

void TM_SPI1_INT_InitPins(TM_SPI_PinsPack_t pinspack);

void TM_SPI_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack) {
	/* Init with default settings */
#ifdef SPI1
	if (SPIx == SPI1) {
		TM_SPIx_Init(SPI1, pinspack, TM_SPI1_MODE, TM_SPI1_PRESCALER, TM_SPI1_MASTERSLAVE, TM_SPI1_FIRSTBIT);
	}
#endif
}

void TM_SPI_InitWithMode(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode) {
	/* Init with custom mode, 0, 1, 2, 3 */
#ifdef SPI1
	if (SPIx == SPI1) {
		TM_SPIx_Init(SPI1, pinspack, SPI_Mode, TM_SPI1_PRESCALER, TM_SPI1_MASTERSLAVE, TM_SPI1_FIRSTBIT);
	}
#endif

}

void TM_SPI_InitFull(
	SPI_TypeDef* SPIx,              \
	TM_SPI_PinsPack_t pinspack,     \
	uint16_t SPI_BaudRatePrescaler, \
	TM_SPI_Mode_t SPI_Mode_t,       \
	uint16_t SPI_Mode,              \
	uint16_t SPI_FirstBit           \
) {

}

uint16_t TM_SPI_GetPrescalerFromMaxFrequency(SPI_TypeDef* SPIx, uint32_t MAX_SPI_Frequency) {

}

TM_SPI_DataSize_t TM_SPI_SetDataSize(SPI_TypeDef* SPIx, TM_SPI_DataSize_t DataSize) {
	
}

void TM_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = *dataOut++;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Read data register */
		*dataIn++ = *(__IO uint8_t *)&SPIx->DR;
	}
}

void TM_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = *dataOut++;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Read data register */
		(void)*(__IO uint16_t *)&SPIx->DR;
	}
}

void TM_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = dummy;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Save data to buffer */
		*dataIn++ = *(__IO uint8_t *)&SPIx->DR;
	}
}

void TM_SPI_SendMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint16_t* dataIn, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		*(__IO uint16_t *)&SPIx->DR = *dataOut++;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Read data register */
		*dataIn++ = *(__IO uint16_t *)&SPIx->DR;
	}
}

void TM_SPI_WriteMulti16(SPI_TypeDef* SPIx, uint16_t* dataOut, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		*(__IO uint16_t *)&SPIx->DR = *dataOut++;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Read data register */
		(void)*(__IO uint16_t *)&SPIx->DR;
	}
}

void TM_SPI_ReadMulti16(SPI_TypeDef* SPIx, uint16_t* dataIn, uint16_t dummy, uint32_t count) {
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);
	
	/* Wait for previous transmissions to complete if DMA TX enabled for SPI */
	SPI_WAIT_TX(SPIx);
	
	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);
		
		/* Fill output buffer with data */
		SPIx->DR = dummy;
		
		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);
		
		/* Save data to buffer */
		*dataIn++ = SPIx->DR;
	}
}

__weak void TM_SPI_InitCustomPinsCallback(SPI_TypeDef* SPIx, uint16_t AlternateFunction) { 
	/* NOTE: This function Should not be modified, when the callback is needed,
           the TM_SPI_InitCustomPinsCallback could be implemented in the user file
   */
}

/* Private functions */
static void TM_SPIx_Init(SPI_TypeDef* SPIx, TM_SPI_PinsPack_t pinspack, TM_SPI_Mode_t SPI_Mode, uint16_t SPI_BaudRatePrescaler, uint16_t SPI_MasterSlave, uint16_t SPI_FirstBit) {

}

/* Private functions */
#ifdef SPI1
void TM_SPI1_INT_InitPins(TM_SPI_PinsPack_t pinspack) {}

#endif
