/******************************************************************************
 * @file:    main.c
 * @purpose: System and Game Initialization, then set Power-down mode
 * @note:    This version supports the LANDTIGER Emulator
 * @version: V1.00
 * @date:    2021-01-12
 ******************************************************************************/

#include "LPC17xx.h"
#include "game.h"

#ifdef SIMULATOR
/* ScaleFlag needs to visible in order for the emulator to find the symbol
   (can be placed also inside system_LPC17xx.h but since it is RO, it
   needs more work)*/
extern uint8_t ScaleFlag;
#endif

int main(void) {
    SystemInit();                           // System Initialization (i.e., PLL)
    
    GAME_Init();                            // Game Initialization
    
    // Set Power-down mode
    LPC_SC->PCON |= 0x1;                    // Set Power Mode Control bit 0 (PM0)
    LPC_SC->PCON &= ~(0x2);                 // Set Power Mode Control bit 1 (PM1)

    while (1) {
        __ASM("wfi");
    }
}
