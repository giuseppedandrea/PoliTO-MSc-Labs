;
; Architetture dei Sistemi di Elaborazione - 02GOLOV
; Laboratory 3
; program2.s
; Giuseppe D'Andrea
;

        .data
v1:     .double -7.5, 7.6, -3.8, 4.4, 8.1, 1.8, 0.2, -4.6, 5.9, 4.9, 2.8, 1.9, 3.6, 6.2, -1.1, 4.3, -1.0, -8.8, 1.5, 1.5, -10.0, -4.9, -8.4, -4.1, 0.8, 6.0, 8.1, 5.6, -8.7, 3.8
v2:     .double -8.0, 8.0, 8.6, -1.0, 8.6, 9.4, 7.1, 4.1, 5.7, 7.2, -3.5, -8.7, 5.9, 9.0, 9.4, -7.8, -0.7, -9.5, 7.6, -5.2, -8.6, 0.5, -1.3, -8.5, -3.9, -3.0, 8.7, -1.3, -8.6, 4.3
v3:     .double -4.4, 0.3, 7.5, 6.2, -6.2, 3.2, 5.3, 6.9, -6.2, 2.9, -2.8, -4.0, 0.2, 8.3, 6.1, 3.3, -6.5, 1.0, -0.6, 4.6, -2.6, 1.8, -1.5, -4.2, 1.4, -5.2, -2.2, 3.4, -1.6, 8.5
v4:     .double 5.5, -2.1, 9.4, 6.2, -8.4, 7.5, -0.6, 1.3, -9.8, 1.8, -9.7, -7.1, -0.1, 7.2, -1.9, 2.7, 7.0, 3.0, -2.0, 8.5, 1.3, -0.4, 5.5, -0.7, -0.6, 2.0, 5.1, -6.5, 6.9, -9.0
v5:     .space 240
v6:     .space 240
v7:     .space 240

        .text
main:   daddui R30, R0, 30      ; n = 30
        daddui R1, R0, 0        ; i = 0
        daddui R5, R0, 0        ; offset = 0
for:    l.d F1, v1(R5)          ; load v1[i]
        l.d F2, v2(R5)          ; load v2[i]
        l.d F3, v3(R5)          ; load v3[i]
        sub.d F8, F1, F2        ; v1[i] - v2[i]
        mul.d F5, F8, F3        ; v5[i] = (v1[i] - v2[i]) * v3[i]
        s.d F5, v5(R5)          ; store v5[i]
        mul.d F6, F5, F3        ; v6[i] = v5[i] * v3[i]
        s.d F6, v6(R5)          ; store v6[i]
        l.d F4, v4(R5)          ; load v4[i]
        add.d F9, F6, F4        ; v6[i] + v4[i]
        mul.d F7, F9, F2        ; v7[i] = (v6[i] + v4[i]) * v2[i]
        s.d F7, v7(R5)          ; store v7[i]
        daddui R5, R5, 8        ; offset += 8
        daddui R1, R1, 1        ; i++
        bne R1, R30, for        ; branch to for if i != 30
        halt                    ; end