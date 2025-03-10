/******************************************************************************
 * @file:    IRQ_timer.c
 * @purpose: Functions to handle TIMER interrupt
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "timer.h"

#include "LPC17xx.h"
#include "../LED/led.h"
#include "../game.h"

/**
 * @brief  TIMER0 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER0_IRQHandler(void) {
    LED_Toggle(GAME_GetDirectionLED());
    
    LPC_TIM0->IR = 1;                       // Clear interrupt flag

    return;
}

/**
 * @brief  TIMER1 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER1_IRQHandler(void) {
    LED_Toggle(GAME_GetRunningLED());
    
    LPC_TIM1->IR = 1;                       // Clear interrupt flag

    return;
}

/**
 * @brief  TIMER2 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER2_IRQHandler(void) {
    LED_ToggleAll();
    
    LPC_TIM2->IR = 1;                       // Clear interrupt flag

    return;
}

/**
 * @brief  TIMER3 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER3_IRQHandler(void) {
    // Action
    
    LPC_TIM3->IR = 1;                       // Clear interrupt flag

    return;
}
