/******************************************************************************
 * @file:    lib_led.h
 * @purpose: Atomic functions to be used by higher SW levels
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "led.h"

#include "LPC17xx.h"

/**
 * @brief  Init LEDs
 *
 * @param  none
 * @return none
 */
void LED_Init(void) {
    LPC_PINCON->PINSEL4 &= 0xFFFF0000;      // PIN mode GPIO (00b value per P2.0 to P2.7)
    
    LPC_GPIO2->FIODIR |= 0x000000FF;        // Pin P2.0...P2.7 (LEDs) defined as Output
    
    LPC_GPIO2->FIOCLR = 0x000000FF;         // All LEDs off
    
    return;
}

/**
 * @brief  DeInit LEDs
 *
 * @param  none
 * @return none
 */
void LED_DeInit(void) {
    LPC_GPIO2->FIOCLR = 0x000000FF;         // All LEDs off
    
    LPC_GPIO2->FIODIR &= 0xFFFFFF00;        // Pin P2.0...P2.7 (LEDs) defined as Input
    
    return;
}
