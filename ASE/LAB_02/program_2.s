;
; Architetture dei Sistemi di Elaborazione - 02GOLOV
; Laboratory 2
; program_2.s
; Giuseppe D'Andrea
;

        .data
v1:     .double -9.3, -1.9, -1.0, -8.8, 6.8, -9.6, -7.4, -5.0, 1.2, -6.4, 8.1, 5.5, 8.0, -5.1, 5.5, 8.2, -4.3, -3.0, 9.4, -4.7, 3.0, 7.0, -6.0, 3.6, -5.5
v2:     .double -6.9, -9.1, -8.1, -3.4, 3.8, -2.4, -9.5, -10.0, 3.8, 3.2, 5.7, -2.4, 8.3, 7.4, -2.5, 6.1, 6.1, 7.9, 2.5, -5.6, -3.9, -8.3, 6.3, 0.5, 1.0
v3:     .space 200
v4:     .double 3.9, 2.7, -7.5, -9.8, -4.1, 5.8, -0.7, 3.0, -3.6, 3.3, -2.3, 8.6, -7.4, -5.9, 7.9, 4.4, -2.1, 5.8, -6.9, -9.4, -8.4, -8.6, 4.8, 6.2, 1.3
v5:     .space 200

        .text
main:   daddui R5, R0, 5        ; n_columns = 5
        daddui R1, R0, 0        ; i = 0
for_i:  daddui R2, R0, 0        ; j = 0
        dmulu R3, R1, R5        ; i * n_columns
for_j:  daddu R4, R3, R2        ; (i * n_columns) + j
        dsll R4, R4, 3          ; ((i * n_columns) + j) * 8
        l.d F1, v1(R4)          ; load v1[i][j]
        l.d F2, v2(R4)          ; load v2[i][j]
        mul.d F3, F1, F2        ; v3[i][j] = v1[i][j] * v2[i][j]
        s.d F3, v3(R4)          ; store v3[i][j]
        l.d F4, v4(R4)          ; load v4[i][j]
        mul.d F5, F3, F4        ; v5[i][j] = v3[i][j] * v4[i][j]
        s.d F5, v5(R4)          ; store v5[i][j]
        daddui R2, R2, 1        ; j++
        bne R2, R5, for_j       ; branch to for_j if j != 5
        daddui R1, R1, 1        ; i++
        bne R1, R5, for_i       ; branch to for_i if i != 5
        halt                    ; end

; Code Optimization
; 580 c.c.
;        .text
;main:   daddui R1, R0, 200      ; offset = 200
;loop:   daddui R1, R1, -8       ; offset -= 8
;        l.d F1, v1(R1)          ; load v1[i][j]
;        l.d F2, v2(R1)          ; load v2[i][j]
;        l.d F4, v4(R1)          ; load v4[i][j]
;        mul.d F3, F1, F2        ; v3[i][j] = v1[i][j] * v2[i][j]
;        mul.d F5, F3, F4        ; v5[i][j] = v3[i][j] * v4[i][j]
;        s.d F3, v3(R1)          ; store v3[i][j]
;        s.d F5, v5(R1)          ; store v5[i][j]
;        bnez R1, loop           ; branch to loop if offset != 0
;        halt                    ; end