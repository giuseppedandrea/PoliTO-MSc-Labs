/******************************************************************************
 * @file:    IRQ_button.c
 * @purpose: Functions to handle External INTerrupt (EINT)
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "button.h"

#include "LPC17xx.h"

uint32_t button_down[3];
uint32_t button_up[3];

/**
 * @brief  External INTerrupt 0 (EINT0) handler
 *
 * @param  none
 * @return none
 */
void EINT0_IRQHandler(void) {
    BUTTON_Disable(0);                      // Disable EINT0
    
    button_down[0] = 1;
    button_up[0] = 0;
    
    LPC_SC->EXTINT &= (1 << 0);             // Clear interrupt flag

    return;
}

/**
 * @brief  External INTerrupt 1 (EINT1) handler
 *
 * @param  none
 * @return none
 */
void EINT1_IRQHandler(void) {
    BUTTON_Disable(1);                      // Disable EINT1
    
    button_down[1] = 1;
    button_up[1] = 0;

    LPC_SC->EXTINT &= (1 << 1);             // Clear interrupt flag

    return;
}

/**
 * @brief  External INTerrupt 2 (EINT2) handler
 *
 * @param  none
 * @return none
 */
void EINT2_IRQHandler(void) {
    BUTTON_Disable(2);                      // Disable EINT2

    button_down[2] = 1;
    button_up[2] = 0;
    
    LPC_SC->EXTINT &= (1 << 2);             // Clear interrupt flag

    return;
}
