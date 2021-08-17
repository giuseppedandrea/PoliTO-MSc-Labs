/******************************************************************************
 * @file:    lib_joystick.h
 * @purpose: Atomic functions to be used by higher SW levels
 * @version: V1.00
 * @date:    2021-01-20
 ******************************************************************************/

#include "joystick.h"

#include "LPC17xx.h"

/**
 * @brief  Init JOYSTICK
 *
 * @param  none
 * @return none
 */
void JOYSTICK_Init(void) {
    LPC_PINCON->PINSEL3 &= ~(3 << 18);      // PIN mode GPIO (00b value per P1.25)
    
    LPC_GPIO1->FIODIR   &= ~(1 << 25);      // Pin P1.25 direction: input
    
    return;
}
