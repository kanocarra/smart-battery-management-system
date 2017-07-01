
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

// Common headers
#define RETURN_VALUE_SUCCESS 1
#define RETURN_VALUE_FAILURE 0

// LED control function
#define RLED1 0
#define RLED2 1
#define GLED1 2
#define GLED2 3

enum LED_mode
{
	LED_On,
	LED_Off,
	LED_Toggle
};

void LED_Control(uint8_t, enum LED_mode);
void MX_GPIO_Init(void);

#endif /*__ pinoutConfig_H */

