/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           led.h
** Descriptions:        Prototypes of functions included in the lib_led, funct_led .c files
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#ifndef LED_H
#define LED_H

/* lib_led */
void LED_init(void);
void LED_deinit(void);

/* funct_led */
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value);

#endif
