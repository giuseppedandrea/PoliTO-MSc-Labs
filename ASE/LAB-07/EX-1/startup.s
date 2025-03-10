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
Stack_Mem       SPACE   Stack_Size
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


; Constants Data (Read Only)

                AREA    CONSTANTS, DATA, READONLY

; Encrypted message
Ciphertext      DCB     "PBOAEOXDKXNYSVMYBCYNSKBMRSDODDEB"
                DCB     "OOCSCDOWSNSOVKLYBKJSYXORYMKZSDYM"
                DCB     "ROSVMSPBKBSYNSMOCKBOCSBYWZOPKMSV"
                DCB     "WOXDOZOBAEOCDYWYDSFYOFSDOBYNSECK"
                DCB     "BVYZOBZBYDOQQOBOSWSOSNKDSCOXCSLS"
                DCB     "VS", 0

; Encrypted message (first 32 bytes)
;Ciphertext      DCB     "PBOAEOXDKXNYSVMYBCYNSKBMRSDODDEB", 0

; Divina Commedia/Inferno/Canto I - Key = 10
;Ciphertext      DCB     "XOVWOJJYNOVMKWWSXNSXYCDBKFSDKWSB"
;                DCB     "SDBYFKSZOBEXKCOVFKYCMEBKMROVKNSB"
;                DCB     "SDDKFSKOBKCWKBBSDKKRSAEKXDYKNSBA"
;                DCB     "EKVOBKOMYCKNEBKOCDKCOVFKCOVFKQQS"
;                DCB     "KOKCZBKOPYBDOMROXOVZOXCSOBBSXYFK"
;                DCB     "VKZKEBKDKXDOKWKBKMROZYMYOZSEWYBD"
;                DCB     "OWKZOBDBKDDKBNOVLOXMRSFSDBYFKSNS"
;                DCB     "BYNOVKVDBOMYCOMRSFRYCMYBDOSYXYXC"
;                DCB     "YLOXBSNSBMYWSFSXDBKSDKXDOBKZSOXN"
;                DCB     "SCYXXYKAEOVZEXDYMROVKFOBKMOFSKKL"
;                DCB     "LKXNYXKSWKZYSMRSPESKVZSONEXMYVVO"
;                DCB     "QSEXDYVKNYFODOBWSXKFKAEOVVKFKVVO"
;                DCB     "MROWKFOKNSZKEBKSVMYBMYWZEXDYQEKB"
;                DCB     "NKSSXKVDYOFSNSVOCEOCZKVVOFOCDSDO"
;                DCB     "QSKNOBKQQSNOVZSKXODKMROWOXKNBSDD"
;                DCB     "YKVDBESZOBYQXOMKVVOKVVYBPEVKZKEB"
;                DCB     "KEXZYMYAEODKMROXOVVKQYNOVMYBWOBK"
;                DCB     "NEBKDKVKXYDDOMRSZKCCKSMYXDKXDKZS"
;                DCB     "ODKOMYWOAEOSMROMYXVOXKKPPKXXKDKE"
;                DCB     "CMSDYPEYBNOVZOVKQYKVKBSFKCSFYVQO"
;                DCB     "KVKMAEKZOBSQVSYCKOQEKDKMYCSVKXSW"
;                DCB     "YWSYMRKXMYBPEQQSFKCSFYVCOKBODBYK"
;                DCB     "BSWSBKBVYZKCCYMROXYXVKCMSYQSKWKS"
;                DCB     "ZOBCYXKFSFKZYSMROSZYCKDYEXZYMYSV"
;                DCB     "MYBZYVKCCYBSZBOCSFSKZOBVKZSKQQSK"
;                DCB     "NSCOBDKCSMROVZSOPOBWYCOWZBOOBKVZ"
;                DCB     "SELKCCYONOMMYAEKCSKVMYWSXMSKBNOV"
;                DCB     "OBDKEXKVYXJKVOQQOBKOZBOCDKWYVDYM"
;                DCB     "RONSZOVWKMYVKDYOBKMYFOBDKOXYXWSC"
;                DCB     "SZKBDSKNSXKXJSKVFYVDYKXJSWZONSFK"
;                DCB     "DKXDYSVWSYMKWWSXYMRSPESZOBBSDYBX"
;                DCB     "KBZSEFYVDOFYVDYDOWZOBKNKVZBSXMSZ"
;                DCB     "SYNOVWKDDSXYOVCYVWYXDKFKXCEMYXAE"
;                DCB     "OVVOCDOVVOMROBKXMYXVESAEKXNYVKWY"
;                DCB     "BNSFSXYWYCCONSZBSWKAEOVVOMYCOLOV"
;                DCB     "VOCSMRKLOXOCZOBKBWOBKMKQSYXONSAE"
;                DCB     "OVVKPSOBKKVKQKODDKZOVVOVYBKNOVDO"
;                DCB     "WZYOVKNYVMOCDKQSYXOWKXYXCSMROZKE"
;                DCB     "BKXYXWSNOCCOVKFSCDKMROWKZZKBFONE"
;                DCB     "XVOYXOAEOCDSZKBOKMROMYXDBKWOFOXS"
;                DCB     "CCOMYXVKDOCDKVDKOMYXBKLLSYCKPKWO"
;                DCB     "CSMROZKBOKMROVKOBOXODBOWOCCOONEX"
;                DCB     "KVEZKMRONSDEDDOLBKWOCOWLSKFKMKBM"
;                DCB     "KXOVKCEKWKQBOJJKOWYVDOQOXDSPOQSK"
;                DCB     "FSFOBQBKWOAEOCDKWSZYBCODKXDYNSQB"
;                DCB     "KFOJJKMYXVKZKEBKMRECMSKNSCEKFSCD"
;                DCB     "KMRSYZOBNOSVKCZOBKXJKNOVKVDOJJKO"
;                DCB     "AEKVOAEOSMROFYVYXDSOBSKMAESCDKOQ"
;                DCB     "SEQXOVDOWZYMROZOBNOBVYPKMOMROXDE"
;                DCB     "DDSCEYSZOXCSOBZSKXQOOCKDDBSCDKDK"
;                DCB     "VWSPOMOVKLOCDSKCKXJKZKMOMROFOXOX"
;                DCB     "NYWSXMYXDBYKZYMYKZYMYWSBSZSQXOFK"
;                DCB     "VKNYFOVCYVDKMOWOXDBOMRSBYFSXKFKS"
;                DCB     "XLKCCYVYMYNSXKXJSKVSYMMRSWSCSPEY"
;                DCB     "PPOBDYMRSZOBVEXQYCSVOXJSYZKBOKPS"
;                DCB     "YMYAEKXNYFSNSMYCDESXOVQBKXNSCOBD"
;                DCB     "YWSCOBOBONSWOQBSNKSKVESAEKVMRODE"
;                DCB     "CSSYNYWLBKYNYWYMOBDYBSCZEYCOWSXY"
;                DCB     "XYWYYWYQSKPESOVSZKBOXDSWSOSPEBYX"
;                DCB     "VYWLKBNSWKXDYKXSZOBZKDBSKKWLONES"
;                DCB     "XKMAESCELSEVSYKXMYBMROPYCCODKBNS"
;                DCB     "OFSCCSKBYWKCYDDYVLEYXYKEQECDYXOV"
;                DCB     "DOWZYNOVSNOSPKVCSOLEQSKBNSZYODKP"
;                DCB     "ESOMKXDKSNSAEOVQSECDYPSQVSEYVNKX"
;                DCB     "MRSCOMROFOXXONSDBYSKZYSMROVCEZOB"
;                DCB     "LYSVSYXPEMYWLECDYWKDEZOBMROBSDYB"
;                DCB     "XSKDKXDKXYSKZOBMROXYXCKVSSVNSVOD"
;                DCB     "DYCYWYXDOMROZBSXMSZSYOMKQSYXNSDE"
;                DCB     "DDKQSYSKYBCODEAEOVFSBQSVSYOAEOVV"
;                DCB     "KPYXDOMROCZKXNSNSZKBVKBCSVKBQYPS"
;                DCB     "EWOBSCZEYCSYVESMYXFOBQYQXYCKPBYX"
;                DCB     "DOYNOVSKVDBSZYODSYXYBOOVEWOFKQVS"
;                DCB     "KWSVVEXQYCDENSYOVQBKXNOKWYBOMROW"
;                DCB     "RKPKDDYMOBMKBVYDEYFYVEWODECOVYWS"
;                DCB     "YWKOCDBYOVWSYKEDYBODECOCYVYMYVES"
;                DCB     "NKMESYDYVCSVYLOVVYCDSVYMROWRKPKD"
;                DCB     "DYYXYBOFONSVKLOCDSKZOBMESYWSFYVC"
;                DCB     "SKSEDKWSNKVOSPKWYCYCKQQSYMROVVKW"
;                DCB     "SPKDBOWKBVOFOXOOSZYVCSKDOMYXFSOX"
;                DCB     "DOXOBOKVDBYFSKQQSYBSCZEYCOZYSMRO"
;                DCB     "VKQBSWKBWSFSNOCOFEYMKWZKBNOCDYVY"
;                DCB     "MYCOVFKQQSYMROAEOCDKLOCDSKZOBVKA"
;                DCB     "EKVDEQBSNOXYXVKCMSKKVDBESZKCCKBZ"
;                DCB     "OBVKCEKFSKWKDKXDYVYWZONSCMOMROVE"
;                DCB     "MMSNOORKXKDEBKCSWKVFKQSKOBSKMROW"
;                DCB     "KSXYXOWZSOVKLBKWYCKFYQVSKONYZYVZ"
;                DCB     "KCDYRKZSEPKWOMROZBSKWYVDSCYXVSKX"
;                DCB     "SWKVSKMESCKWWYQVSKOZSECKBKXXYKXM"
;                DCB     "YBKSXPSXMROVFOVDBYFOBBKMROVKPKBK"
;                DCB     "WYBSBMYXNYQVSKAEOCDSXYXMSLOBKDOB"
;                DCB     "BKXOZOVDBYWKCKZSOXJKKWYBOOFSBDED"
;                DCB     "OOCEKXKJSYXCKBKDBKPOVDBYOPOVDBYN"
;                DCB     "SAEOVVKEWSVOSDKVSKPSKCKVEDOZOBME"
;                DCB     "SWYBSVKFOBQSXOMKWWSVVKOEBSKVYODE"
;                DCB     "BXYOXSCYNSPOBEDOAEOCDSVKMKMMOBKZ"
;                DCB     "OBYQXOFSVVKPSXMROVKFBKBSWOCCKXOV"
;                DCB     "YXPOBXYVKYXNOXFSNSKZBSWKNSZKBDSV"
;                DCB     "VKYXNSYZOBVYDEYWOZOXCYONSCMOBXYM"
;                DCB     "RODEWSCOQESOSYCKBYDEKQESNKODBKBB"
;                DCB     "YDDSNSAESZOBVYMYODDOBXYYFOENSBKS"
;                DCB     "VONSCZOBKDOCDBSNKFONBKSVSKXDSMRS"
;                DCB     "CZSBSDSNYVOXDSMRKVKCOMYXNKWYBDOM"
;                DCB     "SKCMEXQBSNKOFONOBKSMYVYBMROCYXMY"
;                DCB     "XDOXDSXOVPYMYZOBMROCZOBKXNSFOXSB"
;                DCB     "OAEKXNYMROCSKKVOLOKDOQOXDSKVOAEK"
;                DCB     "SZYSCODEFYBBKSCKVSBOKXSWKPSKKMSY"
;                DCB     "ZSENSWONOQXKMYXVOSDSVKCMOBYXOVWS"
;                DCB     "YZKBDSBOMROAEOVVYSWZOBKNYBMROVKC"
;                DCB     "EBOQXKZOBMRSPEBSLOVVKXDOKVKCEKVO"
;                DCB     "QQOXYXFEYVMROXCEKMSDDKZOBWOCSFOQ"
;                DCB     "XKSXDEDDOZKBDSSWZOBKOAESFSBOQQOA"
;                DCB     "ESFSOVKCEKMSDDKOVKVDYCOQQSYYRPOV"
;                DCB     "SMOMYVESMESFSOVOQQOOSYKVESZYODKS"
;                DCB     "YDSBSMROQQSYZOBAEOVVYNSYMRODEXYX"
;                DCB     "MYXYCMOCDSKMMSYMRSYPEQQKAEOCDYWK"
;                DCB     "VOOZOQQSYMRODEWSWOXSVKNYFYBNSMOC"
;                DCB     "DSCSMRSYFOQQSKVKZYBDKNSCKXZSODBY"
;                DCB     "OMYVYBMESDEPKSMYDKXDYWOCDSKVVYBC"
;                DCB     "SWYCCOOSYVSDOXXSNSODBY", 0


; Variables Data (Read/Write)

Freq_Size       EQU     0x00000068                  ; 26 words
Lett_Size       EQU     0x0000001A                  ; 26 bytes

                AREA    VARIABLES, DATA, READWRITE

Frequencies     SPACE   Freq_Size
Letters         SPACE   Lett_Size


                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler               [WEAK]
                LDR     R12, =Reset_Handler

                LDR     R0, =Ciphertext
                LDR     R1, =Frequencies
                LDR     R2, =Letters

                ; Fill Frequencies array
                ; Frequencies[0] = total number of 'A' in Ciphertext,
                ; Frequencies[1] = total number of 'B' in Ciphertext,
                ; Frequencies[2] = total number of 'C' in Ciphertext, ...
freq_loop       LDRB    R3, [R0], #1                ; R3 = Ciphertext[i++]
                CBZ     R3, freq_loop_end           ; if R3 == '\0', break the loop
                SUB     R3, R3, #0x41               ; index = R3 - 'A'
                LDR     R4, [R1, R3, LSL #2]        ; R4 = Frequencies[index]
                ADD     R4, R4, #1                  ; R4++
                STR     R4, [R1, R3, LSL #2]        ; Frequencies[index] = R4
                B       freq_loop
freq_loop_end

                ; Initialize Letters array
                ; Letters[0] = 'A',
                ; Letters[1] = 'B',
                ; Letters[2] = 'C', ...
                MOV     R3, #0
lett_loop       ADD     R4, R3, #0x41
                STRB    R4, [R2, R3]
                ADD     R3, R3, #1
                CMP     R3, #Lett_Size
                BNE     lett_loop

                ; Sort Frequencies array and the corresponding Letters array
                MOV     R3, #Lett_Size              ; array length  = len
bsort_iteration MOV     R4, #0                      ; swaps counter = cnt
                MOV     R5, #0                      ; current index = c_idx
                MOV     R6, #1                      ; next index    = n_idx
bsort_loop      CMP     R6, R3                      ; compare c_idx and len
                BEQ     bsort_loop_end              ; if n_idx == len, end the loop
                LDR     R7, [R1, R5, LSL #2]        ; R7 = Frequencies[c_idx]
                LDR     R8, [R1, R6, LSL #2]        ; R8 = Frequencies[n_idx]
                CMP     R7, R8                      ; compare Frequencies[c_idx] and Frequencies[n_idx]
                BHS     no_swap                     ; if Frequencies[c_idx] >= Frequencies[n_idx], no swap
                STR     R8, [R1, R5, LSL #2]        ; if Frequencies[c_idx] < Frequencies[n_idx], Frequencies[c_idx] = R8
                STR     R7, [R1, R6, LSL #2]        ; if Frequencies[c_idx] < Frequencies[n_idx], Frequencies[n_idx] = R7
                ADD     R4, R4, #1                  ; if Frequencies[c_idx] < Frequencies[n_idx], cnt++
                LDRB    R9, [R2, R5]                ; if Frequencies[c_idx] < Frequencies[n_idx], R9  = Letters[c_idx]
                LDRB    R10,[R2, R6]                ; if Frequencies[c_idx] < Frequencies[n_idx], R10 = Letters[n_idx]
                STRB    R10,[R2, R5]                ; if Frequencies[c_idx] < Frequencies[n_idx], Letters[c_idx] = R10
                STRB    R9, [R2, R6]                ; if Frequencies[c_idx] < Frequencies[n_idx], Letters[n_idx] = R9
no_swap         ADD     R5, R5, #1                  ; c_idx++
                ADD     R6, R6, #1                  ; n_idx++
                B       bsort_loop                  ; go to the next loop
bsort_loop_end  CMP     R4, #0                      ; compare cnt and 0
                SUBNE   R3, R3, #1                  ; if cnt != 0, skip last value in next iteration
                BNE     bsort_iteration             ; if cnt != 0, go to the next iteration

                BX      R12
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
                B       .
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