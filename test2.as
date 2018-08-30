
LOOP: jmp   END
jmp L1(N,W)
MAIN:   mov r1, LENGTH
    add r2,STR


    prn #-5
    sub r1, r4
    inc K
    mov r4,r3
    bne LOOP

STR: .string "abcdef"
LENGTH: .data   6,-9,1, 10, -200, 324, 8, 88, 69, 0, 15000
END:    stop
K:  .data   2
