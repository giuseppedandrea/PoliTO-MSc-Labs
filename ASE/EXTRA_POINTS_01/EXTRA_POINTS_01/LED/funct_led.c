/******************************************************************************
 * @file:    funct_led.h
 * @purpose: High level LED management functions
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "led.h"

#include "LPC17xx.h"

#define LED_N 8

static const uint32_t LED_MASK[] = {1UL << 0, 1UL << 1, 1UL << 2, 1UL << 3, 1UL << 4, 1UL << 5, 1UL << 6, 1UL << 7};

/**
 * @brief  Turn on LED
 *
 * @param  uint8_t LED Number
 * @return none
 */
void LED_On(uint8_t led_num) {
    LPC_GPIO2->FIOPIN |= LED_MASK[led_num];

    return;
}

/**
 * @brief  Turn on all LEDs
 *
 * @param  none
 * @return none
 */
void LED_OnAll(void) {
    uint8_t i;

    for (i = 0; i < LED_N; i++) {
        LED_On(i);
    }

    return;
}

/**
 * @brief  Turn off LED
 *
 * @param  uint8_t LED Number
 * @return none
 */
void LED_Off(uint8_t led_num) {
    LPC_GPIO2->FIOPIN &= ~LED_MASK[led_num];

    return;
}

/**
 * @brief  Turn off all LEDs
 *
 * @param  none
 * @return none
 */
void LED_OffAll(void) {
    uint8_t i;

    for (i = 0; i < LED_N; i++) {
        LED_Off(i);
    }

    return;
}

/**
 * @brief  Toggle LED
 *
 * @param  uint8_t LED Number
 * @return none
 */
void LED_Toggle(uint8_t led_num) {
    if (LPC_GPIO2->FIOPIN & LED_MASK[led_num]) {                // LED is on
        LED_Off(led_num);
    } else {                                                    // LED is off
        LED_On(led_num);
    }

    return;
}

/**
 * @brief  Toggle all LEDs
 *
 * @param  none
 * @return none
 */
void LED_ToggleAll(void) {
    uint8_t i;

    for (i = 0; i < LED_N; i++) {
        LED_Toggle(i);
    }

    return;
}

/**
 * @brief  Outputs value to LEDs
 *
 * @param  uint8_t Value
 * @return none
 */
void LED_Out(uint8_t value) {
    uint8_t i;

    for (i = 0; i < LED_N; i++) {
        if (value & (1 << i)) {
            LED_On(i);
        } else {
            LED_Off(i);
        }
    }

    return;
}
