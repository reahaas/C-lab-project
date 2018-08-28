LENGTH:     .data    6,-9,15
LENGTH:     .data    6 ,-9 ,15
MAIN: mov r3 ,LENGTH

prn #-5
sub    r1,r4
bne END


END: stop
STR: .string "abcdef"
