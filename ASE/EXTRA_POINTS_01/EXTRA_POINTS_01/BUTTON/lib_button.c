/******************************************************************************
 * @file:    lib_button.h
 * @purpose: Atomic functions to be used by higher SW levels
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "button.h"

#include "LPC17xx.h"

const IRQn_Type BUTTON_NVIC[] = {EINT0_IRQn, EINT1_IRQn, EINT2_IRQn};

/**
 * @brief  Init Button
 *
 * @param  uint8_t Button Number (0, 1 or 2)
 * @return none
 */
void BUTTON_Init(uint8_t button_num) {
    if (button_num <= 2) {
        LPC_GPIO2->FIODIR &= ~(1 << (10 + button_num));                             // Pin P2.1{button_num} direction: input

        LPC_SC->EXTMODE |= (1 << button_num);                                       // EINT{button_num} is edge sensitive.
    }

    return;
}

/**
 * @brief  Enable Button interrupt
 *
 * @param  uint8_t Button Number (0, 1 or 2)
 * @return none
 */
void BUTTON_Enable(uint8_t button_num) {
    if (button_num <= 2) {
        LPC_PINCON->PINSEL4 |= (1 << (20 + (button_num << 1)));                     // Pin P2.1{button_num} function: EINT{button_num}

        NVIC_EnableIRQ(BUTTON_NVIC[button_num]);                                    // Enable EINT{button_num} in NVIC interrupt controller
    }

    return;
}

/**
 * @brief  Disable Button interrupt
 *
 * @param  uint8_t Button Number (0, 1 or 2)
 * @return none
 */
void BUTTON_Disable(uint8_t button_num) {
    if (button_num <= 2) {
        NVIC_DisableIRQ(BUTTON_NVIC[button_num]);                                   // Disable EINT{button_num} in NVIC interrupt controller

        LPC_PINCON->PINSEL4 &= ~(3 << (20 + (button_num << 1)));                    // Pin P2.1{button_num} function: GPIO Port 2.1{button_num}
    }

    return;
}

/**
 * @brief  Set Button priority
 *
 * @param  uint8_t Button Number (0, 1 or 2)
 * @param  uint8_t Priority to set (Each interrupt can have one of 32 priorities, where 0 is the highest priority.)
 * @return none
 */
void BUTTON_SetPriority(uint8_t button_num, uint8_t priority) {
    if (button_num <= 2) {
        NVIC_SetPriority(BUTTON_NVIC[button_num], priority);                        // Set EINT{button_num} priority
    }

    return;
}
