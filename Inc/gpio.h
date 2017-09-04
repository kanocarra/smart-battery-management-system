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

void led_flash(int led_combination);
void bal_led_flash(uint8_t led_combination);
void MX_GPIO_Init(void);
void discharge_led(void);

#endif /*__ pinoutConfig_H */
