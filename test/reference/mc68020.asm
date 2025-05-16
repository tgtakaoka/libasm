        cpu     68020
        ;; Wrong opcode
        bfchg   d1{31:32}       ;<EAC1>07C0
        bfclr   d1{31:32}       ;<ECC1>07C0
        bfset   d1{31:32}       ;<EEC1>07C0
        ;; Garbage in unused field
        muls.l  d2,d7           ; 4C02<7800>
        mulu.l  d2,d3           ; 4C02<3800>
