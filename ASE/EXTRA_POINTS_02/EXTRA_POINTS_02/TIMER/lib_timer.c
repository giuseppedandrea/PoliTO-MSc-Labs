/******************************************************************************
 * @file:    lib_timer.h
 * @purpose: Atomic functions to be used by higher SW levels
 * @version: V1.00
 * @date:    2021-01-20
 ******************************************************************************/

#include "timer.h"

#include "LPC17xx.h"

extern uint32_t SystemFrequency;

static const uint32_t PCONP_MASK[] = {1UL << 1, 1UL << 2, 1UL << 22, 1UL << 23};

const IRQn_Type TIMER_NVIC[] = {TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn};

/**
 * @brief  Init TIMER
 *
 * @param  uint8_t TIMER Number (0,1,2,3)
 * @param  uint32_t Time in milliseconds (ms)
 * @param  bool If interrupt is true, an interrupt is generated when MR0 matches the value in the TC.
 * @param  bool If reset is true, the TC will be reset if MR0 matches it
 * @param  bool If stop is true, the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC
 * @return none
 */
void TIMER_Init(uint8_t timer_num, uint32_t time, bool interrupt, bool reset, bool stop) {
    LPC_TIM_TypeDef *LPC_TIM;
    IRQn_Type TIMER_IRQn;
    
    switch (timer_num) {
        case 0:
            LPC_TIM = LPC_TIM0;
            TIMER_IRQn = TIMER0_IRQn;
            break;
        case 1:
            LPC_TIM = LPC_TIM1;
            TIMER_IRQn = TIMER1_IRQn;
            break;
        case 2:
            LPC_TIM = LPC_TIM2;
            TIMER_IRQn = TIMER2_IRQn;
            break;
        case 3:
            LPC_TIM = LPC_TIM3;
            TIMER_IRQn = TIMER3_IRQn;
            break;
        default:
            break;
    }
    
    if (timer_num <= 3) {
        LPC_SC->PCONP |= PCONP_MASK[timer_num];                 // Enable Power Control for TIMER{0,1,2,3}
        
        LPC_TIM->MR0 = (uint32_t)((time / 1000.0) * (SystemFrequency >> 2));        // Set Match Register 0 (MR0) to Time * Frequency(CCLK/4) = ([ms] / 1000) * [Hz] = [s] * [Hz]
        
        LPC_TIM->MCR |= (interrupt);                            // If interrupt is true, then enable interrupt on MR0
        
        LPC_TIM->MCR |= (reset << 1);                           // If reset is true, then reset on MR0
        
        LPC_TIM->MCR |= (stop << 2);                            // If stop is true, then stop on MR0
        
        NVIC_EnableIRQ(TIMER_IRQn);                             // Enable TIMER{0,1,2,3} in NVIC interrupt controller
    }
    
    return;
}

/**
* @brief  Enable TIMER: Timer Counter is enabled for counting
 *
 * @param  uint8_t TIMER Number (0,1,2,3)
 * @return none
 */
void TIMER_Enable(uint8_t timer_num) {
    switch (timer_num) {
        case 0:
            LPC_TIM0->TCR = 1;
            break;
        case 1:
            LPC_TIM1->TCR = 1;
            break;
        case 2:
            LPC_TIM2->TCR = 1;
            break;
        case 3:
            LPC_TIM3->TCR = 1;
            break;
        default:
            break;
    }
    
    return;
}

/**
* @brief  Disable TIMER: Timer Counter is disabled for counting
 *
 * @param  uint8_t TIMER Number (0,1,2,3)
 * @return none
 */
void TIMER_Disable(uint8_t timer_num) {
    switch (timer_num) {
        case 0:
            LPC_TIM0->TCR = 0;
            break;
        case 1:
            LPC_TIM1->TCR = 0;
            break;
        case 2:
            LPC_TIM2->TCR = 0;
            break;
        case 3:
            LPC_TIM3->TCR = 0;
            break;
        default:
            break;
    }
    
    return;
}

/**
* @brief  Reset TIMER: Timer Counter is reset on the next positive edge of PCLK
 *
 * @param  uint8_t TIMER Number (0,1,2,3)
 * @return none
 */
void TIMER_Reset(uint8_t timer_num) {
    switch (timer_num) {
        case 0:
            LPC_TIM0->TCR |= 2;
            break;
        case 1:
            LPC_TIM1->TCR |= 2;
            break;
        case 2:
            LPC_TIM2->TCR |= 2;
            break;
        case 3:
            LPC_TIM3->TCR |= 2;
            break;
        default:
            break;
    }
    
    return;
}

/**
 * @brief  Set TIMER priority
 *
 * @param  uint8_t TIMER Number (0,1,2,3)
 * @param  uint8_t Priority to set (Each interrupt can have one of 32 priorities, where 0 is the highest priority.)
 * @return none
 */
void TIMER_SetPriority(uint8_t timer_num, uint8_t priority) {
    if (timer_num <= 3) {
        NVIC_SetPriority(TIMER_NVIC[timer_num], priority);                          // Set TIMER{timer_num} priority
    }
    
    return;
}
