/******************************************************************************
 * @file:    button.h
 * @brief:   Prototypes of functions included in the lib_button and IRQ_button
 *           .c files
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/**
  \brief  lib_button.c: Atomic functions to be used by higher SW levels
 */
void BUTTON_Init(uint8_t button_num);
void BUTTON_Enable(uint8_t button_num);
void BUTTON_Disable(uint8_t button_num);
void BUTTON_SetPriority(uint8_t button_num, uint8_t priority);

/**
  \brief  IRQ_button.c: Functions to handle External INTerrupt (EINT)
 */
void EINT0_IRQHandler(void);
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);

/**
  \brief  IRQ_button.c: Global variable
 */
extern uint32_t button_down[3];
extern uint32_t button_up[3];

#endif
