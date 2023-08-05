;comment
;another one
MAIN:   mov @r3 ,LENGTH
LOOP:   jmp L1
        mcro m1
        sub @r1, @r4
        bne END
        endmcro
        prn -5
        bne LOOP
        m1
L1:     inc K
        bne LOOP
        mcro m2
        sub @r2, @r3
        bne END
        endmcro 
        m2
END:    stop 
K:      .data 22, 0
L1: .data 2
K1:     .extern E
LENGTH: .data 6,-9 , 15, 3
L1: .data 2
STR:    .string  “abcdef is a string”