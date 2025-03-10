/******************************************************************************
 * @file:    led.h
 * @brief:   Prototypes of functions included in the lib_led and funct_led
 *           .c files
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
  \brief  lib_led.c: Atomic functions to be used by higher SW levels
 */
void LED_Init(void);
void LED_DeInit(void);

/**
  \brief  funct_led.c: High level LED management functions
 */
void LED_On(uint8_t led_num);
void LED_OnAll(void);
void LED_Off(uint8_t led_num);
void LED_OffAll(void);
void LED_Toggle(uint8_t led_num);
void LED_ToggleAll(void);
void LED_Out(uint8_t value);

#endif
