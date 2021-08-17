/******************************************************************************
 * @file:    lib_RIT.h
 * @purpose: Atomic functions to be used by higher SW levels
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "RIT.h"

#include "LPC17xx.h"

extern uint32_t SystemFrequency;

/**
 * @brief  Init Repetitive Interrupt Timer (RIT)
 *
 * @param  uint32_t Time in milliseconds (ms)
 * @param  bool Enable clear: the timer will be cleared to 0 whenever the counter value equals the masked compare value
 * @param  bool Enable debug: the timer is halted when the processor is halted for debugging
 * @return none
 */
void RIT_Init(uint32_t time, bool enable_clear, bool enable_debug) {
    LPC_SC->PCLKSEL1 &= ~(3 << 26);                             // Reset Peripheral Clock Selection for RIT
    LPC_SC->PCLKSEL1 |= (1 << 26);                              // Set Peripheral Clock Selection for RIT to CCLK
    
    LPC_SC->PCONP |= (1 << 16);                                 // Enable Power Control for RIT

    LPC_RIT->RICOMPVAL = (uint32_t)((time / 1000.0) * SystemFrequency);             // Set RI Compare Value to Time * Frequency(CCLK) = ([ms] / 1000) * [Hz] = [s] * [Hz]
    LPC_RIT->RICTRL |= (enable_clear << 1);                     // If enable_clear is true, then enable timer clear
    LPC_RIT->RICTRL |= (enable_debug << 2);                     // If enable_debug is true, then enable timer debug
    LPC_RIT->RICTRL &= ~(1 << 3);                               // Timer disabled
    LPC_RIT->RICOUNTER = 0;                                     // Set RI Counter to 0

    NVIC_EnableIRQ(RIT_IRQn);                                   // Enable RIT in NVIC interrupt controller

    return;
}

/**
 * @brief  Enable Repetitive Interrupt Timer (RIT)
 *
 * @param  none
 * @return none
 */
void RIT_Enable(void) {
    LPC_RIT->RICTRL |= (1 << 3);                                // Timer enabled

    return;
}

/**
 * @brief  Disable Repetitive Interrupt Timer (RIT)
 *
 * @param  none
 * @return none
 */
void RIT_Disable(void) {
    LPC_RIT->RICTRL &= ~(1 << 3);                               // Timer disabled

    return;
}

/**
 * @brief  Reset Repetitive Interrupt Timer (RIT)
 *
 * @param  none
 * @return none
 */
void RIT_Reset(void) {
    LPC_RIT->RICOUNTER = 0;                                     // Set RI Counter to 0

    return;
}

/**
 * @brief  Set RIT priority
 *
 * @param  uint8_t Priority to set (Each interrupt can have one of 32 priorities, where 0 is the highest priority.)
 * @return none
 */
void RIT_SetPriority(uint8_t priority) {
    NVIC_SetPriority(RIT_IRQn, priority);          // Set TIMER{timer_num} priority

    return;
}
