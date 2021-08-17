/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_button.c
** Descriptions:        Button interrupt request handlers
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

signed char count;

void EINT0_IRQHandler (void) {
    count = 0;
	LED_Out(count);
    LPC_SC->EXTINT &= (1 << 0);         // clear pending interrupt
}

void EINT1_IRQHandler (void) {
    if (count < 127) {
        LED_Out(++count);
    }
	LPC_SC->EXTINT &= (1 << 1);         // clear pending interrupt
}

void EINT2_IRQHandler (void) {
    if (count > -128) {
        LED_Out(--count);
    }
    LPC_SC->EXTINT &= (1 << 2);         // clear pending interrupt
}
