/******************************************************************************
 * @file:    RIT.h
 * @brief:   Prototypes of functions included in the lib_RIT and IRQ_RIT .c
 *           files
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>
#include <stdbool.h>

/**
  \brief  lib_RIT.c: Atomic functions to be used by higher SW levels
 */
void RIT_Init(uint32_t time, bool enable_clear, bool enable_debug);
void RIT_Enable(void);
void RIT_Disable(void);
void RIT_Reset(void);
void RIT_SetPriority(uint8_t priority);

/**
  \brief  IRQ_RIT.c: Functions to handle Repetitive Interrupt Timer (RIT)
 */
void RIT_IRQHandler(void);

#endif
