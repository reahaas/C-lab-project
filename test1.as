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
