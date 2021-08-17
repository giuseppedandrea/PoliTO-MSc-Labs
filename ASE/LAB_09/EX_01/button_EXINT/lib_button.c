/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_button.c
** Descriptions:        Atomic button init functions
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "button.h"
#include "lpc17xx.h"

/*----------------------------------------------------------------------------
  Function that initializes BUTTONs
 *----------------------------------------------------------------------------*/

void BUTTON_init(void) {
    LPC_PINCON->PINSEL4    |= (1 << 20);    // External interrupt 0 pin selection
    LPC_GPIO2->FIODIR      &= ~(1 << 10);   // PORT2.10 defined as input

    LPC_PINCON->PINSEL4    |= (1 << 22);    // External interrupt 1 pin selection
    LPC_GPIO2->FIODIR      &= ~(1 << 11);   // PORT2.11 defined as input
  
    LPC_PINCON->PINSEL4    |= (1 << 24);    // External interrupt 2 pin selection
    LPC_GPIO2->FIODIR      &= ~(1 << 12);   // PORT2.12 defined as input

    LPC_SC->EXTMODE = 0x7;

    NVIC_EnableIRQ(EINT2_IRQn);             // enable irq in nvic
    NVIC_EnableIRQ(EINT1_IRQn);             // enable irq in nvic
    NVIC_EnableIRQ(EINT0_IRQn);             // enable irq in nvic
}
