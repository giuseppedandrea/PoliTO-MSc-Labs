;
; Architetture dei Sistemi di Elaborazione - 02GOLOV
; Laboratory 2
; program3.s
; Giuseppe D'Andrea
;

        .data
a:      .byte 15, 8, 6, 5, 1, 9, 1, 11, 7, 13, 9, 12, 8, 0, 5, 11, 8, 4, 6, 13, 7, 15, 3, 0, 1, 2, 4, 3, 2, 15
b:      .byte 2, 11, 8, 9, 7, 0, 5, 5, 13, 12, 1, 13, 14, 15, 14, 7, 15, 9, 4, 9, 13, 4, 11, 7, 8, 13, 3, 8, 5, 11
res:    .space 30

        .text
main:   daddui R5, R0, 30       ; n = 0
        daddui R1, R0, 0        ; i = 0
for:    lb R11, a(R1)           ; load a[i]
        lb R12, b(R1)           ; load b[i]
        daddui R13, R0, 0       ; res[i] = 0
while:  beqz R12, next          ; branch to next if b[i] == 0
        andi R2, R12, 1         ; x = b[i] & 1
        beqz R2, even           ; branch to even if x == 0
        daddu R13, R13, R11     ; res[i] = res[i] + a[i]
even:   dsll R11, R11, 1        ; a[i] = a[i] * 2
        dsrl R12, R12, 1        ; b[i] = b[i] / 2
        j while                 ; jump to while
next:   sb R13, res(R1)         ; store res[i]
        daddui R1, R1, 1        ; i++
        bne R1, R5, for         ; branch to for if i != 30
        halt                    ; end