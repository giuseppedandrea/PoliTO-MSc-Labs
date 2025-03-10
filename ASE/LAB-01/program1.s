;
; Architetture dei Sistemi di Elaborazione - 02GOLOV
; Laboratory 1
; program1.s
; Giuseppe D'Andrea
;

        .data
a:      .word16 2, -31, 1, 7, -5, 31, -10, 11, -5, 25, 18, -5, 26, -15, 15, -32, -1, -27, -23, 14, 16, 28, 17, 8, -27, -14, 6, -25, 27, 4
b:      .word16 -7, -6, 26, -22, -17, -7, 8, -1, 21, -19, 29, -12, 14, -32, -21, -23, -26, -23, -6, -19, 4, 11, -10, 0, -17, -22, -31, -23, -12, 21
c:      .space 60
max:    .space 2
min:    .space 2

        .text
main:   daddui R1, R0, 0        ; offset initialization
        daddui R2, R0, 30       ; index initialization
        daddi R11, R0, 0x8000   ; max initialization
        daddi R12, R0, 0x7fff   ; min initialization
loop:   lh R3, a(R1)            ; load a[i]
        lh R4, b(R1)            ; load b[i]
        daddi R2, R2, -1        ; index decrement
        dadd R5, R3, R4         ; c[i] = a[i] + b[i]
        dsll R5, R5, 2          ; c[i] = c[i] * 4
        slt R7, R5, R11         ; x = c[i] < max
        sh R5, c(R1)            ; store c[i]
        bnez R7, no_max         ; branch if x != 0
        nop                     ; required for the delay slot
        dadd R11, R0, R5        ; max = c[i]
no_max: slt R7, R12, R5         ; x = min < c[i]
        daddui R1, R1, 2        ; offset increment
        bnez R7, no_min         ; branch if x != 0
        nop                     ; required for the delay slot
        dadd R12, R0, R5        ; min = c[i]
no_min: bnez R2, loop           ; branch if index != 0
        sh R11, max(R0)         ; store max
        sh R12, min(R0)         ; store min
        halt                    ; end