.entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne W
sub @r1, @r4
bne L3
L1: inc K
.entry LOOP
jmp W
END: stop
LENGTH: .data 6,-9, 15
Q: .extern LO
K: .data 22
STR: .string "abcdef"

