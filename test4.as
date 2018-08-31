LOOP: jmp L1(#-1,r6)
MAIN: mov r3 ,LENGTH
    .entry LENGTH
    .extern W
prn #-5
bne W(r4,r5)
sub r1, r4
bne L3
L1: inc K
    .entry LOOP
bne LOOP(K,W)
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
    .extern L3
