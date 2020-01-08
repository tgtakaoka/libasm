	CPU     TMS9900
	ORG     1000H

sym1000:        equ     1000H
sym1234:        equ     1234H
sym4000:        equ     4000H
sym5678:        equ     5678H
sym89AB:        equ     89ABH
sym9876:        equ     9876H
zero:           equ     0
offset2:        equ     2
off127:         equ     127
off_128:        equ     -128
size7:          equ     7
neg2:           equ     -2
xop10:          equ    10

        IDLE
        RSET
        RTWP
        CKON
        CKOF
        LREX

        LWPI 1234H
        LIMI 89ABH


        LWPI sym1234
        LIMI sym89AB

        LST  R0
        LWP  R1
        STWP R14
        STST R15

        LI   R0,0000H
        AI   R1,1
        ANDI R8,00FFH
        ORI  R14,0FF00H
        CI   R15,-1

        LI R2,sym1234

        SRA R1,R0
        SRL R4,12
        SLA R8,4
        SRC R9,15

        DIVS R2
        DIVS *R3
        DIVS @1234H
        DIVS @1000H(R4)
        DIVS *R5+
        MPYS R0
        MPYS @2(R8)
        MPYS *R15+

        BLWP @9876H
        B    R13
        X    *R10
        CLR  *R12+
        NEG  R0
        INV  @1234H
        INC  @2(R7)
        INCT R7
        DEC  @-2(R7)
        DECT R7
        BL   R13
        SWPB *R1
        SETO R12
        ABS @8(R3)

        DIVS @sym1234
        DIVS @sym1000(R4)
        BLWP @sym9876
        DEC  @neg2(R7)

        COC R1,R2
        CZC @1234H(R3),R7
        XOR @2(R5),R4
        MPY R4,R2
        DIV R14,R12

        CZC @sym1234(R3),R7
        XOR @offset2(R5),R4

        LDCR *R13+,16
        STCR @2(R4),15

        STCR @offset2(R4),size7
        STCR @offset2(R4),16
        STCR @1000H(R4),size7

        XOP @9876H,0
        XOP @9876H,15

        XOP @sym9876,xop10
        XOP @sym9876(R1),8
        XOP @1234H(R1),xop10

        SZC  @1234H(R10),@5678H(R11)
        SZCB @1234H,@5678H
        S    *R10,*R11
        SB   *R10+,*R11+
        C    *R10+,*R10+
        CB   *R10+,*R11+
        A    @2000H,R11
        AB   R10,@4000H(R11)
        MOV  @0(R10),@1(R11)
        MOVB R10,R11
        SOC  @1234H,@5678H(R11)
        SOCB @1234H(R10),@5678H

        SZC  @sym1234(R10),@sym5678(R11)
        SZCB @sym1234,@sym5678
        AB   R10,@sym4000(R11)
        MOV  @zero(R10),@1(R11)
        SOC  @sym1234,@sym5678(R11)
        SOCB @sym1234(R10),@sym5678

        SBO 0
        SBZ 7FH
        TB  -128

        SBO zero
        SBZ off127
        TB  off_128

	ORG     2000H
        JMP     2002H

        ORG     2100H
        JLT     2100H

	ORG     2200H
        JLE     2200H

        ORG     2300H
        JEQ     2202H

        ORG     2400H
        JHE     2404H
        JGT     2402H
        JNE     2408H
        JNC     240AH
        JOC     23FEH
        JNO     23FCH
        JL      23FAH
        JH      23F8H
        JOP     23F6H

sym3002:        equ     3002H
sym3100:        equ     3100H
sym3200:        equ     3200H
sym3202:        equ     3202H

        ORG     3000H
        JMP     sym3002

        ORG     3100H
        JLT     sym3100

        ORG     3200H
        JLE     sym3200

        ORG     3300H
        JEQ     sym3202
