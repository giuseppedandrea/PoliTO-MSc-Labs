;
; Architetture dei Sistemi di Elaborazione - 02GOLOV
; Laboratory 3
; program_2_c.s
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
main:   daddui R30, R0, 10      ; n = 10
        daddui R1, R0, 0        ; i = 0
        daddui R5, R0, 0        ; offset0 = 0
        daddui R15, R0, 80      ; offset1 = 80
        daddui R25, R0, 160     ; offset2 = 160
for:    l.d F1, v1(R5)          ; load v1[i]
        l.d F2, v2(R5)          ; load v2[i]
        l.d F3, v3(R5)          ; load v3[i]
        sub.d F8, F1, F2        ; v1[i] - v2[i]
        l.d F11, v1(R15)        ; load v1[i + 10]
        l.d F12, v2(R15)        ; load v2[i + 10]
        l.d F13, v3(R15)        ; load v3[i + 10]
        mul.d F5, F8, F3        ; v5[i] = (v1[i] - v2[i]) * v3[i]
        sub.d F18, F11, F12     ; v1[i + 10] - v2[i + 10]
        l.d F21, v1(R25)        ; load v1[i + 20]
        l.d F22, v2(R25)        ; load v2[i + 20]
        l.d F23, v3(R25)        ; load v3[i + 20]
        mul.d F15, F18, F13     ; v5[i + 10] = (v1[i + 10] - v2[i + 10]) * v3[i + 10]
        sub.d F28, F21, F22     ; v1[i + 20] - v2[i + 20]
        l.d F4, v4(R5)          ; load v4[i]
        mul.d F6, F5, F3        ; v6[i] = v5[i] * v3[i]
        s.d F5, v5(R5)          ; store v5[i]
        mul.d F25, F28, F23     ; v5[i + 20] = (v1[i + 20] - v2[i + 20]) * v3[i + 20]
        l.d F14, v4(R15)        ; load v4[i + 10]
        l.d F24, v4(R25)        ; load v4[i + 20]
        mul.d F16, F15, F13     ; v6[i + 10] = v5[i + 10] * v3[i + 10]
        s.d F15, v5(R15)        ; store v5[i + 10]
        daddui R1, R1, 1        ; i++
        add.d F9, F6, F4        ; v6[i] + v4[i]
        s.d F6, v6(R5)          ; store v6[i]
        mul.d F26, F25, F23     ; v6[i + 20] = v5[i + 20] * v3[i + 20]
        s.d F25, v5(R25)        ; store v5[i + 20]
        mul.d F7, F9, F2        ; v7[i] = (v6[i] + v4[i]) * v2[i]
        add.d F19, F16, F14     ; v6[i + 10] + v4[i + 10]
        s.d F16, v6(R15)        ; store v6[i + 10]
        mul.d F17, F19, F12     ; v7[i + 10] = (v6[i + 10] + v4[i + 10]) * v2[i + 10]
        add.d F29, F26, F24     ; v6[i + 20] + v4[i + 20]
        s.d F26, v6(R25)        ; store v6[i + 20]
        mul.d F27, F29, F22     ; v7[i + 20] = (v6[i + 20] + v4[i + 20]) * v2[i + 20]
        s.d F7, v7(R5)          ; store v7[i]
        daddui R5, R5, 8        ; offset0 += 8
        s.d F17, v7(R15)        ; store v7[i + 10]
        daddui R15, R15, 8      ; offset1 += 8
        s.d F27, v7(R25)        ; store v7[i + 20]
        bne R1, R30, for        ; branch to for if i != 10
        daddui R25, R25, 8      ; offset2 += 8
        halt                    ; end