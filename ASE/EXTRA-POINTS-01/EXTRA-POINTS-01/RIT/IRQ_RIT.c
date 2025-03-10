/******************************************************************************
 * @file:    IRQ_RIT.c
 * @purpose: Functions to handle Repetitive Interrupt Timer (RIT)
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "RIT.h"

#include "LPC17xx.h"
#include "../BUTTON/button.h"
#include "../game.h"

/**
 * @brief  Repetitive Interrupt Timer (RIT) handler
 *
 * @param  none
 * @return none
 */
void RIT_IRQHandler(void) {
    static bool skip[] = {true, true, true};
    
    // START button debouncing
    if (button_down[BUTTON_START] > 0) {
        if (skip[BUTTON_START] == true) {                       // Skip first RIT interrupt after button interrupt because it may be too close (< RIT time interval)
            skip[BUTTON_START] = false;
        } else {
            uint32_t pin_state = LPC_GPIO2->FIOPIN & (1 << (10 + BUTTON_START));
            if (pin_state == 0) {
                button_up[BUTTON_START] = 0;
                button_down[BUTTON_START]++;
                if (button_down[BUTTON_START] == 2) {           // Button pressed
                    GAME_Start();
                }
            } else {
                if (button_down[BUTTON_START] < 2) {            // Bounce
                    button_down[BUTTON_START] = 0;
                    button_up[BUTTON_START] = 0;
                    skip[BUTTON_START] = true;
                    BUTTON_Enable(BUTTON_START);
                } else {
                    button_up[BUTTON_START]++;
                    if (button_up[BUTTON_START] == 2) {         // Button released
                        button_down[BUTTON_START] = 0;
                        button_up[BUTTON_START] = 0;
                        skip[BUTTON_START] = true;
                    }
                }
            }
        }
    }

    // ROTATE button debouncing
    if (button_down[BUTTON_ROTATE] > 0) {
        if (skip[BUTTON_ROTATE] == true) {                      // Skip first RIT interrupt after button interrupt because it may be too close (< RIT time interval)
            skip[BUTTON_ROTATE] = false;
        } else {
            uint32_t pin_state = LPC_GPIO2->FIOPIN & (1 << (10 + BUTTON_ROTATE));
            if (pin_state == 0) {
                button_up[BUTTON_ROTATE] = 0;
                button_down[BUTTON_ROTATE]++;
            } else {                                            
                if (button_down[BUTTON_ROTATE] < 2) {           // Bounce
                    button_down[BUTTON_ROTATE] = 0;
                    button_up[BUTTON_ROTATE] = 0;
                    skip[BUTTON_ROTATE] = true;
                    BUTTON_Enable(BUTTON_ROTATE);
                } else {
                    button_up[BUTTON_ROTATE]++;
                    if (button_up[BUTTON_ROTATE] == 2) {        // Button released
                        GAME_RobotRotate();
                        button_down[BUTTON_ROTATE] = 0;
                        button_up[BUTTON_ROTATE] = 0;
                        skip[BUTTON_ROTATE] = true;
                        BUTTON_Enable(BUTTON_ROTATE);
                    }
                }
            }
        }
    }
    
    // RUN button debouncing
    if (button_down[BUTTON_RUN] > 0) {
        if (skip[BUTTON_RUN] == true) {                         // Skip first RIT interrupt after button interrupt because it may be too close (< RIT time interval)
            skip[BUTTON_RUN] = false;
        } else {
            uint32_t pin_state = LPC_GPIO2->FIOPIN & (1 << (10 + BUTTON_RUN));
            if (pin_state == 0) {                               
                button_up[BUTTON_RUN] = 0;
                button_down[BUTTON_RUN]++;
                if (button_down[BUTTON_RUN] >= 2) {             // Button pressed and hold
                    GAME_RobotMove();
                }
            } else {
                if (button_down[BUTTON_RUN] < 2) {              // Bounce
                    button_down[BUTTON_RUN] = 0;
                    button_up[BUTTON_RUN] = 0;
                    skip[BUTTON_RUN] = true;
                    BUTTON_Enable(BUTTON_RUN);
                } else {
                    button_up[BUTTON_RUN]++;
                    if (button_up[BUTTON_RUN] == 2) {           // Button released
                        GAME_RobotStop();
                        button_down[BUTTON_RUN] = 0;
                        button_up[BUTTON_RUN] = 0;
                        skip[BUTTON_RUN] = true;
                        BUTTON_Enable(BUTTON_RUN);
                    }
                }
            }
        }
    }
    
    LPC_RIT->RICTRL |= (1 << 1);                                // Clear interrupt flag

    return;
}
