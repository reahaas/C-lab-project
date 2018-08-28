add r1,r2
bne LOOP(K,W)
MAIN: mov ***, LENGTH
add r2,STR
LOOP: jmp END
prn #-5
sub r1, r4
mov **,r3
bne LOOP
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: add r1 ,r2
W: add r1 ,r2

