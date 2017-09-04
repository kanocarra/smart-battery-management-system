/**
  ******************************************************************************
  * File Name          : led_controller.c
  * Description        : This file provides code to control the onboard LEDs
  ******************************************************************************
*/

#include "gpio.h"

// Turn on LEDs in a binary representation of the combination number
void led_flash(int led_combination){

    // Reset all LEDs
    HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 0); 
    HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 0);

    // Turn on Green LED 1
    if(led_combination % 2) {
        HAL_GPIO_WritePin(GPIOC, GLED1_Pin, 1); 
    }

    // Turn on Green LED 2
    if(led_combination == 2 || led_combination == 3 || led_combination == 6 
       || led_combination == 7 || led_combination == 10 || led_combination == 11
       || led_combination == 15) {

        HAL_GPIO_WritePin(GPIOC, GLED2_Pin, 1); 
    }

    // Turn on Red LED 1
    if((led_combination >= 4 && led_combination <= 7) || led_combination >= 12){
        HAL_GPIO_WritePin(GPIOC, RLED1_Pin, 1); 
    }

    // Turn on Red LED 2
    if(led_combination >= 8) {
        HAL_GPIO_WritePin(GPIOC, RLED2_Pin, 1); 
    }
}

void bal_led_flash(uint8_t led_combination){

    // Reset all LEDs
    HAL_GPIO_WritePin(GPIOC, PA0_Pin, 0); 
    HAL_GPIO_WritePin(GPIOC, PA1_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, PA2_Pin, 0);

    if(led_combination >= 1) {
        HAL_GPIO_WritePin(GPIOC, PA0_Pin, 1); 
    }
    if (led_combination >= 2){
        HAL_GPIO_WritePin(GPIOC, PA1_Pin, 1); 
    } 
    if (led_combination == 3){
        HAL_GPIO_WritePin(GPIOC, PA2_Pin, 1); 
    }

}

void discharge_led(void) {
        // Reset all LEDs
    HAL_GPIO_WritePin(GPIOC, PA0_Pin, 0); 
    HAL_GPIO_WritePin(GPIOC, PA1_Pin, 0);
    HAL_GPIO_WritePin(GPIOC, PA2_Pin, 0);

    // Set all LEDs
    HAL_GPIO_WritePin(GPIOC, PA0_Pin, 1); 
    HAL_GPIO_WritePin(GPIOC, PA1_Pin, 1);
    HAL_GPIO_WritePin(GPIOC, PA2_Pin, 1);

}