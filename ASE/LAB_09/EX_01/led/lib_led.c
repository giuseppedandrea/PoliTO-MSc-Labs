/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_led.c
** Descriptions:        Atomic led init functions
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "led.h"

/*----------------------------------------------------------------------------
  Function that initializes LEDs and switch them off
 *----------------------------------------------------------------------------*/

unsigned char led_value;

void LED_init(void) {
    LPC_PINCON->PINSEL4 &= 0xFFFF0000;	//PIN mode GPIO (00b value per P2.0 to P2.7)
	LPC_GPIO2->FIODIR   |= 0x000000FF;  //P2.0...P2.7 Output (LEDs on PORT2 defined as Output)
	LPC_GPIO2->FIOCLR    = 0x000000FF;  //all LEDs off
    
	led_value = 0;
}

void LED_deinit(void) {
    LPC_GPIO2->FIODIR &= 0xFFFFFF00;    //P2.0...P2.7 Output LEDs on PORT2 defined as Output
}
