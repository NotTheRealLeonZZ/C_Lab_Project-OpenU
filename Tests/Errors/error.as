; file ps.as
TOP: .entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH,
LOOP: jmp, L1
prn -5000
bne W
sub @r1, @r4
bne L3
L1: inc K
.entry LOOPS
jmp W
END: stop_now
STR: .string "abc"def"
LENGTH: .data 6,-9,15, number, a
K: .data 22000
.extern L3