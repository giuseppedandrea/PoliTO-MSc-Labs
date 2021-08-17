;/**************************************************************************//**
; * @file     startup_LPC17xx.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for
; *           NXP LPC17xx Device Series
; * @version  V1.10
; * @date     06. April 2011
; *
; * @note
; * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
                SPACE   Stack_Size / 2
Stack_Process   SPACE   Stack_Size / 2
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp                ; Top of Stack
                DCD     Reset_Handler               ; Reset Handler
                DCD     NMI_Handler                 ; NMI Handler
                DCD     HardFault_Handler           ; Hard Fault Handler
                DCD     MemManage_Handler           ; MPU Fault Handler
                DCD     BusFault_Handler            ; Bus Fault Handler
                DCD     UsageFault_Handler          ; Usage Fault Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     SVC_Handler                 ; SVCall Handler
                DCD     DebugMon_Handler            ; Debug Monitor Handler
                DCD     0                           ; Reserved
                DCD     PendSV_Handler              ; PendSV Handler
                DCD     SysTick_Handler             ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler              ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler           ; 17: Timer0
                DCD     TIMER1_IRQHandler           ; 18: Timer1
                DCD     TIMER2_IRQHandler           ; 19: Timer2
                DCD     TIMER3_IRQHandler           ; 20: Timer3
                DCD     UART0_IRQHandler            ; 21: UART0
                DCD     UART1_IRQHandler            ; 22: UART1
                DCD     UART2_IRQHandler            ; 23: UART2
                DCD     UART3_IRQHandler            ; 24: UART3
                DCD     PWM1_IRQHandler             ; 25: PWM1
                DCD     I2C0_IRQHandler             ; 26: I2C0
                DCD     I2C1_IRQHandler             ; 27: I2C1
                DCD     I2C2_IRQHandler             ; 28: I2C2
                DCD     SPI_IRQHandler              ; 29: SPI
                DCD     SSP0_IRQHandler             ; 30: SSP0
                DCD     SSP1_IRQHandler             ; 31: SSP1
                DCD     PLL0_IRQHandler             ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler              ; 33: Real Time Clock
                DCD     EINT0_IRQHandler            ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler            ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler            ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler            ; 37: External Interrupt 3
                DCD     ADC_IRQHandler              ; 38: A/D Converter
                DCD     BOD_IRQHandler              ; 39: Brown-Out Detect
                DCD     USB_IRQHandler              ; 40: USB
                DCD     CAN_IRQHandler              ; 41: CAN
                DCD     DMA_IRQHandler              ; 42: General Purpose DMA
                DCD     I2S_IRQHandler              ; 43: I2S
                DCD     ENET_IRQHandler             ; 44: Ethernet
                DCD     RIT_IRQHandler              ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler            ; 46: Motor Control PWM
                DCD     QEI_IRQHandler              ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler             ; 48: PLL1 Lock (USB PLL)
                DCD     USBActivity_IRQHandler      ; 49: USB Activity interrupt to wakeup
                DCD     CANActivity_IRQHandler      ; 50: CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler               [WEAK]

                MOV     R0, #3
                MSR     CONTROL, R0
                LDR     SP, =Stack_Process

                ; RESET testbench
                LDR     R0, =0xCAFECAFE
                LDR     R3, =0xFEDEFEDE
                LDR     R5, =0xDEADBEEF

                SVC     0x00                        ; R0 = 0
                SVC     0x03                        ; R3 = 0
                SVC     0x05                        ; R5 = 0

                ; NOP testbench
                SVC     0x10                        ; NOP
                SVC     0xFF                        ; NOP

                ; MEMCMP testbench
                LDR     R0, =test1_memcmp
                LDR     R1, =test1_memcmp
                SVC     0x48                        ; R0 = 0

                LDR     R0, =test1_memcmp
                LDR     R1, =test2_memcmp
                SVC     0x48                        ; R0 = 1

                LDR     R0, =test2_memcmp
                LDR     R1, =test1_memcmp
                SVC     0x48                        ; R0 = -1

stop            B       stop

test1_memcmp    DCB     "ABCDEFGH"
test2_memcmp    DCB     "ABCDABCD"

                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                 [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler           [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler           [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler            [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler          [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                 [WEAK]

                PUSH    {R8-R12, LR}

                TST     LR, #4
                MRSNE   R11, PSP                    ; if LR == 0xFFFFFFFD, R11 = PSP
                MRSEQ   R11, MSP                    ; if LR == 0xFFFFFFF1 || LR == 0xFFFFFFF9, R11 = MSP

                LDR     R10, [R11, #24]
                LDR     R10, [R10, #-4]
                BIC     R10, #0xFF000000
                LSR     R10, #16                    ; R10 = SVC number

                CMP     R10, #8
                BHS     end_RESET                   ; if SVC number > 7, skip RESET functionality
                CMP     R10, #4
                MOVLO   R8, #0
                STRLO   R8, [R11, R10, LSL #2]      ; if SVC number < 4, [PSP/MSP + (SVC number << 2)] = 0
                BLO     end_SVC
                CMP     R10, #4
                MOVEQ   R4, #0                      ; if SVC number == 4, R4 = 0
                CMP     R10, #5
                MOVEQ   R5, #0                      ; if SVC number == 5, R5 = 0
                CMP     R10, #6
                MOVEQ   R6, #0                      ; if SVC number == 6, R6 = 0
                CMP     R10, #7
                MOVEQ   R7, #0                      ; if SVC number == 7, R7 = 0
                B       end_SVC
end_RESET             

                MOV     R8, R10
                LSR     R8, #6                      ; R8 = SVC number[8:6]
                CMP     R8, #1
                BNE     end_MEMCMP                  ; if R8 != 1, skip MEMCMP functionality
                MOV     R9, R10
                AND     R9, #0x0000003F             ; R9 = number of bytes
loop            CMP     R9, #0
                MOVEQ   R0, #0                      ; if R9 == 0, R0 = 0
                BEQ     store_MEMCMP
                SUB     R9, R9, #1                  ; R9--
                LDRB    R2, [R0], #1                ; R2 = [R0++]
                LDRB    R3, [R1], #1                ; R3 = [R1++]
                CMP     R2, R3
                BEQ     loop                        ; if R2 == R3, next compare
                MOVGT   R0, #1                      ; if R2 > R3, R0 = 1
                MOVLT   R0, #-1                     ; if R2 < R3, R0 = -1
store_MEMCMP    STR     R0, [R11]                   ; [PSP/MSP] = R0
                B       end_SVC
end_MEMCMP

end_SVC         POP     {R8-R12, LR}
                BX      LR

                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler            [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler              [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler             [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler              [WEAK]
                EXPORT  TIMER0_IRQHandler           [WEAK]
                EXPORT  TIMER1_IRQHandler           [WEAK]
                EXPORT  TIMER2_IRQHandler           [WEAK]
                EXPORT  TIMER3_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler            [WEAK]
                EXPORT  UART1_IRQHandler            [WEAK]
                EXPORT  UART2_IRQHandler            [WEAK]
                EXPORT  UART3_IRQHandler            [WEAK]
                EXPORT  PWM1_IRQHandler             [WEAK]
                EXPORT  I2C0_IRQHandler             [WEAK]
                EXPORT  I2C1_IRQHandler             [WEAK]
                EXPORT  I2C2_IRQHandler             [WEAK]
                EXPORT  SPI_IRQHandler              [WEAK]
                EXPORT  SSP0_IRQHandler             [WEAK]
                EXPORT  SSP1_IRQHandler             [WEAK]
                EXPORT  PLL0_IRQHandler             [WEAK]
                EXPORT  RTC_IRQHandler              [WEAK]
                EXPORT  EINT0_IRQHandler            [WEAK]
                EXPORT  EINT1_IRQHandler            [WEAK]
                EXPORT  EINT2_IRQHandler            [WEAK]
                EXPORT  EINT3_IRQHandler            [WEAK]
                EXPORT  ADC_IRQHandler              [WEAK]
                EXPORT  BOD_IRQHandler              [WEAK]
                EXPORT  USB_IRQHandler              [WEAK]
                EXPORT  CAN_IRQHandler              [WEAK]
                EXPORT  DMA_IRQHandler              [WEAK]
                EXPORT  I2S_IRQHandler              [WEAK]
                EXPORT  ENET_IRQHandler             [WEAK]
                EXPORT  RIT_IRQHandler              [WEAK]
                EXPORT  MCPWM_IRQHandler            [WEAK]
                EXPORT  QEI_IRQHandler              [WEAK]
                EXPORT  PLL1_IRQHandler             [WEAK]
                EXPORT  USBActivity_IRQHandler      [WEAK]
                EXPORT  CANActivity_IRQHandler      [WEAK]

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                END