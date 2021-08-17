/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           button.h
** Descriptions:        Prototypes of functions included in the lib_button, IRQ_button .c files
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

/* lib_button */
void BUTTON_init(void);

/* IRQ_button */
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

#endif
