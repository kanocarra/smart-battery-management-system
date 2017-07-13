
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"
#include <stdint.h>

typedef enum {OFF, IDLE, START, MEASURE, ESTIMATE_SOC, COMPUTE_R, COMPUTE_C, BAL, SEND, SHUTDOWN} Led_Sequence;

// Common headers
#define RETURN_VALUE_SUCCESS 1
#define RETURN_VALUE_FAILURE 0

void led_flash(int led_combination);
void MX_GPIO_Init(void);

#endif /*__ pinoutConfig_H */

