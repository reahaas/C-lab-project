MAIN:   mov ***, LENGTH
    add r2,STR
LOOP: jmp   END
    prn #-5
    sub r1, r4
    inc K
    mov **,r3
    bne LOOP
END:    stop
STR: .string "abcdef"
LENGTH: .data   6,-9,1, 10, -200, 324, 8, 88, 69, 0, 15000
K:  .data   2
