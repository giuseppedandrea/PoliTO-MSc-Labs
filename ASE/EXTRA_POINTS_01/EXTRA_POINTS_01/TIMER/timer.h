/******************************************************************************
 * @file:    timer.h
 * @brief:   Prototypes of functions included in the lib_timer and IRQ_timer
 *           .c files
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/**
  \brief  lib_timer.c: Atomic functions to be used by higher sw levels
 */
void TIMER_Init(uint8_t timer_num, uint32_t time, bool interrupt, bool reset, bool stop);
void TIMER_Enable(uint8_t timer_num);
void TIMER_Disable(uint8_t timer_num);
void TIMER_Reset(uint8_t timer_num);
void TIMER_SetPriority(uint8_t timer_num, uint8_t priority);

/**
  \brief  IRQ_timer.c: Functions to handle TIMER interrupt
 */
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void TIMER3_IRQHandler(void);

#endif
