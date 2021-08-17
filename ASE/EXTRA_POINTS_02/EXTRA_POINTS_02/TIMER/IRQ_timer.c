/******************************************************************************
 * @file:    IRQ_timer.c
 * @purpose: Functions to handle TIMER interrupt
 * @version: V1.00
 * @date:    2021-01-20
 ******************************************************************************/

#include "timer.h"

#include "LPC17xx.h"
#include "../game.h"
#include "../TP/TouchPanel.h"

// JOYSTICK mask to detect the pressing of only one key at a time (no intermediate direction, e.g. north-east, is allowed)
static const uint32_t JOYSTICK_MASK[] = {0x1E << 25,            // JOYSTICK Select pressed
                                         0x1D << 25,            // JOYSTICK Down pressed
                                         0x1B << 25,            // JOYSTICK Left pressed
                                         0x17 << 25,            // JOYSTICK Right pressed
                                         0x0F << 25};           // JOYSTICK Up pressed

/**
 * @brief  TIMER0 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER0_IRQHandler(void) {
    TP_GetDisplayPoint(&display, TP_ReadAds7846(), &matrix);
    
    if ((display.x <= 240) && (display.x > 0) && (display.y <= 320) && (display.y > 0)) {
        GAME_Touch(display.x, display.y);
    }
    
    LPC_TIM0->IR = 1;                                           // Clear interrupt flag
    
    return;
}

/**
 * @brief  TIMER1 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER1_IRQHandler(void) {
    static uint8_t down[] = {0, 0, 0, 0, 0};
    
    uint32_t pin_state = LPC_GPIO1->FIOPIN & (0x1F << 25);      // Select JOYSTICK bits (25, 26, 27, 28, 29) 
    
    if (pin_state == JOYSTICK_MASK[0]) {                        // JOYSTICK Select
        down[0]++;
        if (down[0] == 1) {
            GAME_ChangeMode();
        }
    } else {
        down[0] = 0;
    }
    
    if (pin_state == JOYSTICK_MASK[1]) {                        // JOYSTICK Down
        down[1]++;
        if (down[1] >= 1) {
            GAME_Action(2);
        }
    } else {
        down[1] = 0;
    }
    
    if (pin_state == JOYSTICK_MASK[2]) {                        // JOYSTICK Left
        down[2]++;
        if (down[2] >= 1) {
            GAME_Action(3);
        }
    } else {
        down[2] = 0;
    }
    
    if (pin_state == JOYSTICK_MASK[3]) {                        // JOYSTICK Right
        down[3]++;
        if (down[3] >= 1) {
            GAME_Action(1);
        }
    } else {
        down[3] = 0;
    }
    
    if (pin_state == JOYSTICK_MASK[4]) {                        // JOYSTICK Up
        down[4]++;
        if (down[4] >= 1) {
            GAME_Action(0);
        }
    } else {
        down[4] = 0;
    }
    
    LPC_TIM1->IR = 1;                                           // Clear interrupt flag
    
    return;
}

/**
 * @brief  TIMER2 interrupt handler
 *
 * @param  none
 * @return none
 */
void TIMER2_IRQHandler(void) {
    // Action
    
    LPC_TIM2->IR = 1;                                           // Clear interrupt flag
    
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
