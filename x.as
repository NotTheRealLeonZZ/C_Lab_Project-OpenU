; file x.as
.entry LENGTH
.extern W
.extern L3
R2: .entry LOOP

MAIN: cmp L3, L3
LOOP: jmp LENGTH
TEST: stop

STR: .string "abcdef"
LENGTH: .data 6,-9, 15




